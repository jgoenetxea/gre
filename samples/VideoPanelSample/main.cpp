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

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "objloader.hpp"
#include "shapes.hpp"

#include "scene.hpp"
#include "transformation.hpp"

#include "renderer.hpp"
#include "projectiveCamera.hpp"
#include "orthoCamera.hpp"

#include "video_panel.hpp"


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
    window = glfwCreateWindow( 1024, 768, "Test window", NULL, NULL );
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

    // get renderer instance
    gre::Renderer* m_renderer = gre::Renderer::getInstance();

    // Generate the main model
    gre::VideoPanel m_vp;

    // Generate translation node
    gre::Transformation m_trans;

    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 500 );
    glm::vec3 up = glm::vec3( 0,1,0 );
    float fov = 60.0;

    gre::ProjectiveCamera m_camera;
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_camera.setConfiguration(fov*GRAD2RAD, 4.0f / 3.0f, 0.1f, 1000.0f);
    // View matrix
    m_camera.setLocation( position,         // Camera is here
                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                          up                // Head is up (set to 0,-1,0 to look upside-down)
                          );

//    // Generate camera instance
//    glm::vec3 position = glm::vec3( 0, 0, 5 );
//    glm::vec3 up = glm::vec3( 0,1,0 );

//    gre::OrthoCamera m_camera;
//    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//    m_camera.setConfiguration(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
//    // View matrix
//    m_camera.setLocation( position,           // Camera is here
//                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
//                          up                  // Head is up (set to 0,-1,0 to look upside-down)
//                          );

    // Generate scene
    gre::Scene m_scene;
    m_scene.addCamera(m_camera);
    m_scene.addChild(&m_trans);
    m_trans.addChild(&m_vp);

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
    double lastTime = glfwGetTime();
    float horizontalAngle = 0.f;
    float translateValue = 0.f;
    int rotSpeed = 80 * GRAD2RAD;
    int transSpeed = 200;
    cv::Mat frame;
    while(!glfwWindowShouldClose(window))
    {
        // Get new frame
        capture.grab();
        capture.retrieve( frame );
        if( flipImageVert ) cv::flip( frame, frame, 0 );
        if( flipImageHori ) cv::flip( frame, frame, 1 );
        m_vp.setFrame( frame.data, frame.cols, frame.rows, frame.channels() );
//        cv::imshow("frame", frame);
//        cv::waitKey(5);

        // time control
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        horizontalAngle += deltaTime * rotSpeed;
        translateValue = cos(deltaTime) * transSpeed;

        m_trans.setRotation(horizontalAngle, glm::vec3(0, 1, 1));
        m_trans.setTranslation(glm::vec3(0, 0, translateValue));

        m_renderer->renderScene(&m_scene);

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
