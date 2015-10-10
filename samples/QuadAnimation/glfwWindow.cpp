#include "glfwWindow.h"
#include  <string>

#include "logger.h"
#define LOG_TAG "GLFWWindowImpl"

GLFWWindowImpl::GLFWWindowImpl()
{
}

GLFWWindowImpl::~GLFWWindowImpl()
{
}

bool GLFWWindowImpl::init(const std::string& windowName, const int windowWidth, const int windowHeight, key_callback_t keyCallbackFunction, error_callback_t errorCallbackFunction)
{
    m_screenWidth = windowWidth;
    m_screenHeighth = windowHeight;

    LOGI("Initializing gre engine...");
    glfwSetErrorCallback(errorCallbackFunction);

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
    m_window = glfwCreateWindow( m_screenWidth, m_screenHeighth, windowName.c_str(), NULL, NULL );
    if( m_window == NULL )
    {
        LOGE("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." );
        glfwTerminate();
        exitWithError(-1);
    }

    glfwMakeContextCurrent(m_window);
    //glfwSwapInterval(1);

    glfwSetKeyCallback(m_window, keyCallbackFunction);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
    LOGI("Engine initialized!");
    return 0;
}

bool GLFWWindowImpl::updateScene()
{
    return glfwWindowShouldClose(m_window);
}

bool GLFWWindowImpl::render()
{
    // Swap buffers
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GLFWWindowImpl::exitWithError(int errorCode)
{
    LOGE("Received error code: %d", errorCode);
    exit(errorCode);
}

void GLFWWindowImpl::close()
{
    // Cleanup VBO and shader
    LOGI("Cleaning...");
    //glDeleteProgram(programID);
    //glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &m_vertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(m_window);
    glfwTerminate();
    LOGI("Terminated!");
}
