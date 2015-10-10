// include opengl
#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>

typedef void (*key_callback_t)(GLFWwindow* window, int key, int scancode, int action, int mods); // type for conciseness
typedef void (*error_callback_t)(int error, const char* description); // type for conciseness

class GLFWWindowImpl
{
public:
    GLFWWindowImpl();
    ~GLFWWindowImpl();

    virtual bool init(const std::string& windowName, const int windowWidth, const int windowHeight, key_callback_t keyCallbackFunction, error_callback_t errorCallbackFunction);

    virtual bool updateScene();

    virtual bool render();

    virtual void exitWithError(int errorCode);

    virtual void close();

    void resizeViewport(int width, int height);

protected:
    int m_screenWidth;
    int m_screenHeighth;
    GLFWwindow* m_window;
    GLuint m_vertexArrayID;
};
