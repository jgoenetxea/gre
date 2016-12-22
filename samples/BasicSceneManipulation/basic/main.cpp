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
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "objloader.hpp"
#include "shapes.hpp"

#include "scene.hpp"
#include "transformation.hpp"

#include "renderer.hpp"
#include "povCamera.hpp"

string assets_path = ASSET_DIRECTORY;

string vShader = assets_path+"shaders/basic130.vert";
string fShader = assets_path+"shaders/basic130.frag";
string uvtemplate = assets_path+"obj/cube.png";
string modelFile = assets_path+"obj/cube.obj";
string uvAxesFile = assets_path+"obj/axes.png";
string axesFile = assets_path+"obj/axes.obj";
string earthFile = assets_path+"obj/Realistic_earth.obj";
string uvEarthFile = assets_path+"obj/Realistic_earth.DDS";

static float userPosX = 0.f;
static float userPosY = 0.f;
static float userPosZ = 1.f;
static float stepSize = 0.1f;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
        userPosZ -= stepSize;
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
        userPosZ += stepSize;
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
        userPosX -= stepSize;
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
        userPosX += stepSize;
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
        userPosY += stepSize;
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
        userPosY -= stepSize;
}

int main( void )
{
    glm::mat3 cameraProjectionMatrix;
    float cameraDistortionMatrix[5];
    float imageHeight = 640.f;
    float imageWidth  = 480.f;
    float fovy = 45.f;
    float near = 2.f;
    float far  = 100.f;
    float asp = imageHeight / imageWidth;

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

    // Generate the main model
    gre::Obj* m_cube = gre::ObjFactory::getInstance()->loadOBJ( modelFile );
    m_cube->setShadersFromFiles( vShader, fShader );
    m_cube->setTexture( uvtemplate );
    m_cube->setName("Planet");
    m_cube->setTranslation(glm::vec3(0, 0, -5));

    glm::mat4 tMat = m_cube->getLocalTranslation();
//    {
//        std::cout << "Local Tranlation Matrix: " << std::endl;
//        const float *pSource = (const float*)glm::value_ptr((tMat));
//        for (int i = 0; i < 4; ++i) {
//            for (int j = 0; j < 4; ++j) {
//                std::cout << pSource[i*4+j] << ", ";
//            }
//            std::cout << std::endl;
//        }
//    }

    gre::POVCamera m_camera;
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //m_camera.setConfiguration(fov*GRAD2RAD, 4.0f / 3.0f, 0.1f, 1000.0f);
    m_camera.setConfiguration(0.f, 0.f, 1, 4.f, 4.f / 3.f);

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

        // m_cube->setRotation(-horizontalAngle/2, glm::vec3(0, 1, 0));

        m_renderer->renderScene(&m_scene);

        m_camera.setConfiguration(userPosX, userPosY, userPosZ, 4.f, 4.f / 3.f);

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
