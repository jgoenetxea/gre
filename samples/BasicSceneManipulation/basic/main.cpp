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

// #define FULL_SCREEN

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "objloader.hpp"
#include "shapes.hpp"

#include "scene.hpp"
#include "transformation.hpp"

#include "renderer.hpp"
#include "projectiveCamera.hpp"

using std::cout;
using std::endl;

string assets_path = ASSET_DIRECTORY;

string vShader = assets_path+"shaders/basic130.vert";
string fShader = assets_path+"shaders/basic130.frag";
string uvtemplate = assets_path+"obj/cube.png";
string modelFile = assets_path+"obj/cube.obj";
string uvAxesFile = assets_path+"obj/axes.png";
string axesFile = assets_path+"obj/axes.obj";
string earthFile = assets_path+"obj/Realistic_earth.obj";
string uvEarthFile = assets_path+"obj/Realistic_earth.DDS";

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods)
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

    GLFWmonitor* monitor_id = NULL;

#if defined(FULL_SCREEN)
    monitor_id = glfwGetPrimaryMonitor();
#endif

	// Open a window and create its OpenGL context
    window = glfwCreateWindow( 800, 600,
                               "Test window",
                               monitor_id, NULL );
    if( window == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
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

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

    // get renderer instance
    gre::Renderer* m_renderer = gre::Renderer::getInstance();

    // Generate the main model
    gre::Obj* m_cube = gre::ObjFactory::getInstance()->loadOBJ( modelFile );
    m_cube->setShadersFromFiles( vShader, fShader );
    m_cube->setTexture( uvtemplate );
    m_cube->setName("Cube");
    m_cube->setTranslation(glm::vec3(0, 0, -17));

    glm::vec3 position = glm::vec3( 0, 0, 0 );
    glm::vec3 target = glm::vec3( 0, 0, -1 );
    glm::vec3 up = glm::vec3( 0,1,0 );

    gre::ProjectiveCamera m_camera;
    float fov = 45.f;
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_camera.setConfiguration(fov, 4.0f / 3.0f, 0.1f, 1000.0f);
    m_camera.setLocation( position,         // Camera is here
                          target,           // and looks here : at the same position, plus "direction"
                          up                // Head is up (set to 0,-1,0 to look upside-down)
                          );

    // Generate scene
    gre::Scene m_scene;
    m_scene.addCamera(m_camera);

    m_scene.addChild(m_cube);
    // Initial position : on +Z
    double lastTime = glfwGetTime();
    float horizontalAngle = 0.f;
    int rotSpeed = 180 * GRAD2RAD;
    while(!glfwWindowShouldClose(window))
    {
        // time control
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        horizontalAngle += deltaTime * rotSpeed;

        m_cube->setRotation(-horizontalAngle/2, glm::vec3(0, 1, 0));

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
