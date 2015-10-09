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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "objloader.hpp"
#include "shapes.hpp"

#include "scene.hpp"
#include "translation.hpp"

#include "renderer.hpp"
#include "projectiveCamera.hpp"

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
    string cubeFile = assets_path+"obj/cube.obj";
    string sphereFile = assets_path+"obj/dirtySphere.obj";
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

std::vector<gre::Scene> m_scene(2);
// Generate translation node
std::vector<gre::Translation> m_trans(2);

void generateScenes()
{
    // Generate the main model
    gre::Obj* m_cube = gre::ObjFactory::getInstance()->loadOBJ( cubeFile );
    m_cube->setShadersFromFiles( vShader, fShader );
    m_cube->setTexture( uvtemplate );

    gre::Obj* m_sphere = gre::ObjFactory::getInstance()->loadOBJ( sphereFile );
    m_sphere->setShadersFromFiles( vShader, fShader );
    m_sphere->setTexture( uvtemplate );

    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    glm::vec3 up = glm::vec3( 0,1,0 );
    float fov = 60.0;

    gre::ProjectiveCamera* m_camera = new gre::ProjectiveCamera();
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_camera->setConfiguration(fov*GRAD2RAD, 4.0f / 3.0f, 0.1f, 100.0f);
    // View matrix
    m_camera->setLocation( position,           // Camera is here
                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                          up                  // Head is up (set to 0,-1,0 to look upside-down)
                          );

    // Generate scene
    m_scene[0].addCamera(*m_camera);
    m_scene[0].addChild(&m_trans[0]);
    m_trans[0].addChild(m_cube);
    // Generate scene
//    m_scene[1].addCamera(*m_camera);
//    m_scene[1].addChild(&m_trans[1]);
//    m_trans[1].addChild(m_sphere);
    // Generate scene
    m_scene[1].addCamera(*m_camera);
    m_scene[1].addChild(&m_trans[1]);
    m_trans[1].addChild(m_cube);
    //m_trans[1].addChild(m_sphere);
    glm::mat4* trans = new glm::mat4(1);
    *trans = glm::translate(*trans, glm::vec3(0, 2, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
    gre::Translation* sphereOffset = new gre::Translation();
    sphereOffset->setLocalTranslation( *trans );
    sphereOffset->addChild(m_cube);
    m_trans[1].addChild(sphereOffset);

    // set node names
    m_camera->setName("Camera");
    m_trans[0].setName("Trans_0");
    m_trans[1].setName("Trans_1");
    sphereOffset->setName("sphereOffset");
    m_cube->setName("Cube mesh");
    m_sphere->setName("Sphere mesh");
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

    generateScenes();

    // Initial position : on +Z
    double lastTime = glfwGetTime();
    float horizontalAngle = 0.f;
    float translateValue = 0.f;
    int rotSpeed = 80*GRAD2RAD;
    int transSpeed = 5;

    int sceneId = 0;
    int counter = 0;
    while(!glfwWindowShouldClose(window))
    {
        // time control
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        if( counter++ % 200 == 0 )
        {
            sceneId = sceneId?0:1;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        horizontalAngle += deltaTime * rotSpeed;
        translateValue += deltaTime * transSpeed;

        glm::mat4 ModelMatrix = glm::mat4(1.0);
        if( sceneId == 0 )
        {
            float radius = 2.f;
            ModelMatrix = glm::rotate(ModelMatrix, horizontalAngle, glm::vec3(0, 1, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
            ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cos(translateValue)*radius, sin(translateValue)*radius, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
        }
        else if( sceneId == 1 )
        {
            float radius = 2.f;
            //ModelMatrix = glm::rotate(ModelMatrix, horizontalAngle, glm::vec3(0, 1, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
            ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cos(translateValue)*radius, 0, sin(translateValue)*radius)); // where x, y, z is axis of rotation (e.g. 0 1 0)
        }

        m_trans[sceneId].setLocalTranslation(ModelMatrix);

        //scene.draw();
        m_renderer->renderScene(&m_scene[sceneId]);

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
