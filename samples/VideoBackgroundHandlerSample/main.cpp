// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RAD2GRAD 180.0/M_PI
#define GRAD2RAD M_PI/180.0

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "videoBackgroundHandler.h"


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main( void )
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
    window = glfwCreateWindow( /*1024*/768, 768, "Test window", NULL, NULL );
    if( window == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

    gre::VideoBackgroundHandler* vbh = gre::VideoBackgroundHandler::getInstance();

    // Init camera capture
    bool flipImageVert = false;
    bool flipImageHori = true;
    std::string inputVideoName = "";
    cv::VideoCapture capture;
    if( inputVideoName == "" )
    {
#ifdef __APPLE__
        capture.open( CV_CAP_QT );
        // Define the camera image size
        //capture.set(CV_CAP_PROP_FRAME_WIDTH,800);
        //capture.set(CV_CAP_PROP_FRAME_HEIGHT,600);

        capture.set(CV_CAP_PROP_FRAME_WIDTH,320);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT,640);
#elif defined(__WINDOWS__)
        capture.open( CV_CAP_DSHOW );
        // Define the camera image size
        capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
#else
        capture.open( 0 );
#endif
    }
    else
    {
        capture.open( inputVideoName );
    }
    // This block is to wait the camera activation
    cv::Mat bigFrame;
    int cont = 0;
    if( capture.isOpened() )
    {
        //printf("Wait until the camera is completely loaded\n");
        bool cameraIsLoaded = false;
        while(!cameraIsLoaded)
        {
            cont += 1;
            capture.read(bigFrame);

            if(!bigFrame.empty())
            {
                //printf("OK!, The camera is working now, so follow the execution\n");
                cameraIsLoaded = true;
                printf("Image size: %dx%d\n", bigFrame.cols, bigFrame.rows);
            }

            int key = cv::waitKey(10);

            if (key == 27 )
            {
                //printf("Well, you want to exit before the camere is ready!\n");
                exit(0);
            }
        }
    }
    else
    {
        std::cout << "Error: Capture not opened!" << std::endl;
        return -1;
    }

    // Initial position : on +Z
    cv::Mat frame;
    while(!glfwWindowShouldClose(window))
    {
        // Get new frame
        capture.grab();
        capture.retrieve( frame );
        cv::cvtColor(frame, frame, CV_BGR2RGB);
        if( flipImageVert ) cv::flip( frame, frame, 0 );
        if( flipImageHori ) cv::flip( frame, frame, 1 );
//        cv::imshow("frame", frame);
//        cv::waitKey(5)

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vbh->setFrame( frame.data, frame.cols, frame.rows, frame.channels() );

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed

	// Cleanup VBO and shader
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
