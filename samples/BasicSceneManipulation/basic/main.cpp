// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../../gre/objloader.hpp"
#include "../../gre/shapes.hpp"

#include "../../gre/scene.hpp"
#include "../../gre/translation.hpp"

#include "../../gre/renderer.hpp"
#include "../../gre/projectiveCamera.hpp"

#ifdef _WIN32
	string vShader = "../TransformVertexShader.vertexshader";
	string fShader = "../TextureFragmentShader.fragmentshader";
	string uvtemplate = "../uvtemplate.DDS";
	string modelFile = "../cube.obj";
#else
    string vShader = "../BasicSceneManipulation/TransformVertexShader.vertexshader";
    string fShader = "../BasicSceneManipulation/TextureFragmentShader.fragmentshader";
    string uvtemplate = "../BasicSceneManipulation/uvtemplate.DDS";
    string modelFile = "../BasicSceneManipulation/cube.obj";
#endif

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

	glfwSetWindowTitle( "<- - ->" );

    //SceneControl* sceneControl;
    //sceneControl = SceneControl::getInstance();
    //sceneControl->setControlType( INTER_MOUSENONE_KEYROTATE );
    //sceneControl->setControlType( INTER_MOUSELOOK_KEYROTATE );
    //sceneControl->setControlType( INTER_MOUSELOOK_KEYTRANSLATE );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwSetMousePos(1024/2, 768/2);

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
    m_camera.setLocation( position,           // Camera is here
                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                          up                  // Head is up (set to 0,-1,0 to look upside-down)
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
    do
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
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
           glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	//glDeleteProgram(programID);
	//glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

    //SceneControl::deleteInstance();

	return 0;
}
