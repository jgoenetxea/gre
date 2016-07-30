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

static gre::VideoBackgroundHandler* _vbh = nullptr;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


static double mouseX = 0;
static double mouseY = 0;
static void onMouseMove(GLFWwindow* window, double x, double y)
{
    mouseX = x;
    mouseY = y;
}

static void onMouseButton(GLFWwindow* window, int button, int action, int w )
{
    printf("Mouse button pressed!!\n");

    if( button==GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE )
    {

    }
    else if( button==GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE )
    {
        float x = static_cast<float>(mouseX);
        float y = static_cast<float>(mouseY);
        _vbh->fromScreenToImageCoordinates(x,y);

        printf("From [%d, %d] to [%f, %f]\n", mouseX, mouseY, x, y);
    }
}

int main( void )
{
    _vbh = gre::VideoBackgroundHandler::getInstance();;

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
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onMouseMove);

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

    // Initial position : on +Z
    cv::Mat frame = cv::Mat(600, 800, CV_8UC3, cv::Scalar(0,255,0));
    // Get new frame
    cv::imshow("frame", frame);
    cv::waitKey(100);

    _vbh->setFrame( frame.data, frame.cols, frame.rows, frame.channels() );

    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _vbh->show();

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
