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

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "objloader.hpp"
#include "shapes.hpp"

#include "scene.hpp"
#include "transformation.hpp"

#include "renderer.hpp"
#include "projectiveCamera.hpp"

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

    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Test window", NULL, NULL );
    if( window == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    //SceneControl* sceneControl;
    //sceneControl = SceneControl::getInstance();
    //sceneControl->setControlType( INTER_MOUSENONE_KEYROTATE );
    //sceneControl->setControlType( INTER_MOUSELOOK_KEYROTATE );
    //sceneControl->setControlType( INTER_MOUSELOOK_KEYTRANSLATE );

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
    gre::Obj* m_planet = gre::ObjFactory::getInstance()->loadOBJ( modelFile );
    m_planet->setShadersFromFiles( vShader, fShader );
    m_planet->setTexture( uvtemplate );
    m_planet->setName("Planet");
    m_planet->setTranslation(glm::vec3(3, 0, 0)); // Set radio

    gre::Obj* m_moon = gre::ObjFactory::getInstance()->loadOBJ( modelFile );
    m_moon->setShadersFromFiles( vShader, fShader );
    m_moon->setTexture( uvtemplate );
    m_moon->setName("Moon");
    m_moon->setScale(glm::vec3(0.3f, 0.3f, 0.3f));
    m_moon->setTranslation(glm::vec3(2, 0, 0)); // Set radio

    //gre::Obj* m_mainOrbital = gre::ObjFactory::getInstance()->loadOBJ( axesFile );
    //m_mainOrbital->setShadersFromFiles( vShader, fShader );
    //m_mainOrbital->setTexture( uvAxesFile );
    //m_mainOrbital->setName("MainOrbital");

    //gre::Obj* m_planetOrbital = gre::ObjFactory::getInstance()->loadOBJ( axesFile );
    //m_planetOrbital->setShadersFromFiles( vShader, fShader );
    //m_planetOrbital->setTexture( uvAxesFile );
    //m_planetOrbital->setName("PlanetOrbital");

    // Generate translation node
    gre::Transformation m_mainOrbital;
    m_mainOrbital.setName("MainOrbital");

    gre::Transformation m_planetOrbital;
    m_planetOrbital.setName("PlanetOrbital");
    m_planetOrbital.setTranslation(glm::vec3(3, 0, 0));

    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 7 );
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

    // Generate scene
    gre::Scene m_scene;
    m_scene.addCamera(m_camera);

    m_scene.addChild(&m_mainOrbital);
    m_mainOrbital.addChild(&m_planetOrbital);

    m_mainOrbital.addChild(m_planet);
    m_planetOrbital.addChild(m_moon);

    m_mainOrbital.setRotation(0.8f, glm::vec3(0, 0, 1));

    // Initial position : on +Z
    double lastTime = glfwGetTime();
    float horizontalAngle = 0.f;
    float translateValue = 0.f;
    float rotationAngle = 0.f;
    int rotSpeed = 180 * GRAD2RAD;
    int orbitationSpeed = 80 * GRAD2RAD;
    int transSpeed = 5;
    while(!glfwWindowShouldClose(window))
    {
        // time control
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        horizontalAngle += deltaTime * rotSpeed;
        rotationAngle += deltaTime * orbitationSpeed;
        translateValue += deltaTime * transSpeed;

        m_moon->setRotation(-horizontalAngle/2, glm::vec3(0, 1, 0));

        m_planetOrbital.setRotation(horizontalAngle, glm::vec3(0, 1, 0));

        m_planet->setRotation(horizontalAngle, glm::vec3(0, 1, 1));

        m_mainOrbital.setRotation(rotationAngle, glm::vec3(0, 0, 1));

        m_renderer->renderScene(&m_scene);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed

	// Cleanup VBO and shader
	//glDeleteProgram(programID);
	//glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
	glfwTerminate();

    //SceneControl::deleteInstance();

	return 0;
}