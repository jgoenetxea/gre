// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RAD2GRAD 180.0/M_PI
#define GRAD2RAD M_PI/180.0

// include opengl
#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "objloader.hpp"
#include "shapes.hpp"

#include "scene.hpp"
#include <transformation.hpp>

#include "renderer.hpp"
#include "projectiveCamera.hpp"
#include "orthoCamera.hpp"

#include "square2D.h"
#include "math2D.h"
using namespace threemonkeybits;

#include "logger.h"
#define LOG_TAG "SAMPLE-ORTHOPANEL"

string assets_path = ASSET_DIRECTORY;

#ifdef _WIN32
    string vShader = "../TransformVertexShader.vertexshader";
    string fShader = "../TextureFragmentShader.fragmentshader";
    string uvtemplate = "../uvtemplate.DDS";
    string modelFile = "../cube.obj";
#else
    string vShader = assets_path+"shaders/basic130.vert";
    string fShader = assets_path+"shaders/basic130.frag";
    string uvtemplate = assets_path+"obj/uvtemplate.DDS";
    string modelFile = assets_path+"obj/cube.obj";
#endif

////////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////////

// Render
const int screenWidth = 1024;
const int screenHeighth = 768;
GLFWwindow* window;
gre::Renderer* m_renderer;
gre::Obj* m_obj;
gre::Transformation m_trans;
gre::Scene m_scene;
GLuint VertexArrayID;
gre::OrthoCamera m_camera;

// Configuration
double lastTime = 0.f;
float horizontalAngle = 0.f;
float translateValue = 0.f;
int rotSpeed = 0;
int transSpeed = 0;

// Scene generated
int maxNumberOfRectangles = 0;
int lowWidth = 0;
int highWidth = 0;
int lowHeight = 0;
int highHeight = 0;
Point2D centerSquares;
float radiusSquares = 0.0f;
std::vector<Square2D*> rectangles;

////////////////////////////////////////////////////////////////////////////////////
// Callbacks
////////////////////////////////////////////////////////////////////////////////////
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
////////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////////
int initEngine();
int initScene();
void configureScene();
int generateScene();
void process();
void exitWithError(const int errorCode);
void cleanAndTerminate();

////////////////////////////////////////////////////////////////////////////////////
// Function
////////////////////////////////////////////////////////////////////////////////////
int initEngine()
{
	LOGI("Initializing gre engine...");
	glfwSetErrorCallback(error_callback);

	// Initialise GLFW
	if( !glfwInit() )
	{
		LOGE( "Failed to initialize GLFW" );
		exitWithError(-1);
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( screenWidth, screenHeighth, LOG_TAG, NULL, NULL );
	if( window == NULL )
	{
		LOGE("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." );
		glfwTerminate();
		exitWithError(-1);
	}

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	LOGI("Engine initialized!");
	return 0;
}

int initScene()
{
	LOGI("Initializing scene...");
	// get renderer instance
	m_renderer = gre::Renderer::getInstance();

	// Generate the main model
	m_obj = gre::ShapeDispatcher::getShapes()->getQuad();
	m_obj->setShadersFromFiles( vShader, fShader );
	m_obj->setTexture( uvtemplate );
	// Generate camera instance
	glm::vec3 position = glm::vec3( 0, 0, 5 );
	glm::vec3 up = glm::vec3( 0,1,0 );


	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_camera.setConfiguration(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
	// View matrix
	m_camera.setLocation( position,           // Camera is here
			glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Generate scene
	m_scene.addCamera(m_camera);
	m_scene.addChild(&m_trans);
	m_trans.addChild(m_obj);
	LOGI("Scene initialized!");
	return 0;
}

/**
 * Initial position : on +Z
 */
void configureScene()
{
	LOGI("Configuring scene...");

	// Scene
    lastTime = glfwGetTime();
    horizontalAngle = 0.f;
    translateValue = 0.f;
    rotSpeed = 80*GRAD2RAD;
    transSpeed = 14;

    // Rectangles
    maxNumberOfRectangles = 10;
    lowWidth = 1;
    highWidth = 5;
    lowHeight = 2;
    highHeight = 5;
    centerSquares = Point2D(0.0f, 0.0f);
    radiusSquares = 5.0f;
    LOGI("Scene configured!");
}
int generateScene()
{
	LOGI("Generating scene...");

	// Create random squares
	for(int i = 0; i < maxNumberOfRectangles; i++)
	{
		float width = (float)math2d::randomNumberInterval(lowWidth, highWidth);
		float height = (float)math2d::randomNumberInterval(lowHeight, highHeight);
		Point2D origin = math2d::calculateRandomPoint2DInACircle(centerSquares, radiusSquares);
		Square2D* square = new Square2D(origin, width, height);
		rectangles.push_back(square);
	}
	// Print info
	for(std::vector<Square2D*>::const_iterator it = rectangles.begin(); it != rectangles.end(); it++)
	{
		std::string data = (*it)->getJSONFormat();
		LOGD("Data: %s", data.c_str());
	}
	LOGI("Scene generated!");
	return 0;
}
int freeScene()
{
	LOGI("Freeing scene memory..");
	for(std::vector<Square2D*>::const_iterator it = rectangles.begin(); it != rectangles.end(); it++)
	{
		delete *it;
	}
	rectangles.clear();
}
void process()
{
	 // time control
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	horizontalAngle += deltaTime * rotSpeed;
	translateValue += deltaTime * transSpeed;

	glm::mat4 ModelMatrix = glm::mat4(1.0);
	//ModelMatrix = glm::rotate(ModelMatrix, horizontalAngle, glm::vec3(1, 0, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)

	m_trans.setLocalTranslation(ModelMatrix);

	//scene.draw();
	m_renderer->renderScene(&m_scene);

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void exitWithError(int errorCode)
{
	LOGE("Received error code: %d", errorCode);
	exit(errorCode);
}
void cleanAndTerminate()
{
    // Cleanup VBO and shader
	LOGI("Cleaning...");
    //glDeleteProgram(programID);
    //glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    LOGI("Terminated!");
}
int main( void )
{
	initEngine();
	initScene();
	configureScene();
	generateScene();

	LOGI("Start to process...");
    while(!glfwWindowShouldClose(window))	// Check if the ESC key was pressed or the window was closed
    {
       process();
    }
    LOGI("Processing finished!");

    freeScene();
    cleanAndTerminate();
    return 0;
}
