// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/objloader.hpp"
#include "common/shapes.hpp"

#include "common/scene.hpp"
#include "common/translation.hpp"

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

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
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

    // Generate the main model
    Obj* m_obj = ObjFactory::getInstance()->loadOBJ( modelFile );
    m_obj->setShaders( vShader, fShader );
    m_obj->setTexture( uvtemplate );

    // Generate translation node
    Translation m_trans;

    // Generate scene
    Scene scene;
    scene.addChild(&m_trans);
    m_trans.addChild(m_obj);

    // Initial position : on +Z
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    glm::vec3 up = glm::vec3( 0,1,0 );
    float fov = 60.0;
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

        // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 ProjectionMatrix = glm::perspective(fov, 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 ViewMatrix       = glm::lookAt(
                                    position,           // Camera is here
                                    glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                                    up                  // Head is up (set to 0,-1,0 to look upside-down)
                               );

        horizontalAngle += deltaTime * rotSpeed;
        translateValue += deltaTime * transSpeed;

        glm::mat4 ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::rotate(ModelMatrix, horizontalAngle, glm::vec3(0, 1, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cos(translateValue), sin(translateValue), 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)

        //m_obj->setLocalTranslation(ModelMatrix);
        m_trans.setLocalTranslation(ModelMatrix);
        //glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        //m_obj->setMVP(MVP);
        //m_obj->setMVP(ModelMatrix, ViewMatrix, ProjectionMatrix);
        m_obj->setVP(ViewMatrix, ProjectionMatrix);

        scene.draw();

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
