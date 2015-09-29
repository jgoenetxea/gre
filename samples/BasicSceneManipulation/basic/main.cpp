// Include standard headers
#include <stdio.h>
#include <stdlib.h>

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
#include "translation.hpp"

#include "renderer.hpp"
#include "projectiveCamera.hpp"

#ifdef _WIN32
	string vShader = "../TransformVertexShader.vertexshader";
	string fShader = "../TextureFragmentShader.fragmentshader";
	string uvtemplate = "../uvtemplate.DDS";
	string modelFile = "../cube.obj";
#else
    string vShader = "/home/VICOMTECH/jgoenetxea/Repositories/git/labirinth/samples/BasicSceneManipulation/TransformVertexShader.vertexshader";
    string fShader = "/home/VICOMTECH/jgoenetxea/Repositories/git/labirinth/samples/BasicSceneManipulation/TextureFragmentShader.fragmentshader";
    string uvtemplate = "/home/VICOMTECH/jgoenetxea/Repositories/git/labirinth/samples/BasicSceneManipulation/uvtemplate.DDS";
    string modelFile = "/home/VICOMTECH/jgoenetxea/Repositories/git/labirinth/samples/BasicSceneManipulation/cube.obj";
#endif

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

    glfwWindowHint(GLFW_DEPTH_BITS, 16);

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
    gre::Obj* m_obj = gre::ObjFactory::getInstance()->loadOBJ( modelFile );
    m_obj->setShadersFromFiles( vShader, fShader );
    m_obj->setTexture( uvtemplate );

    // Generate translation node
    gre::Translation m_trans;

    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    glm::vec3 up = glm::vec3( 0,1,0 );
    float fov = 60.0;

    gre::ProjectiveCamera m_camera;
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_camera.setConfiguration(fov, 4.0f / 3.0f, 0.1f, 100.0f);
    // View matrix
    m_camera.setLocation( position,         // Camera is here
                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                          up                // Head is up (set to 0,-1,0 to look upside-down)
                          );

    // Generate scene
    gre::Scene m_scene;
    m_scene.addCamera(m_camera);
    m_scene.addChild(&m_trans);
    m_trans.addChild(m_obj);

    // Initial position : on +Z
    double lastTime = glfwGetTime();
    float horizontalAngle = 0.f;
    float translateValue = 0.f;
    int rotSpeed = 80;
    int transSpeed = 14;
    while(!glfwWindowShouldClose(window))
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
        ModelMatrix = glm::rotate(ModelMatrix, horizontalAngle, glm::vec3(0, 1, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cos(translateValue), sin(translateValue), 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)

        m_trans.setLocalTranslation(ModelMatrix);

        //scene.draw();
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
