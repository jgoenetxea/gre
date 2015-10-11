#include "rectGenerator.h"

#include <unistd.h>

RectGenerator rg;
////////////////////////////////////////////////////////////////////////////////////
// Callbacks
////////////////////////////////////////////////////////////////////////////////////
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const float step = 0.2f;
    static float currentZoom = 1.f;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        rg.translateCameraPosition(glm::vec3(0.f, step, 0.f));
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        rg.translateCameraPosition(glm::vec3(0.f, -step, 0.f));
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        rg.translateCameraPosition(glm::vec3(-step, 0.f, 0.f));
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        rg.translateCameraPosition(glm::vec3(step, 0.f, 0.f));
    }
    else if (key == GLFW_KEY_PAGE_UP  && action == GLFW_PRESS)
    {
        currentZoom += 0.1f;
        rg.zoom(currentZoom);
    }
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
    {
        currentZoom -= 0.1f;
        rg.zoom(currentZoom);
    }
}

int main( void )
{

    rg.init("Quad generator", 800, 600, key_callback, error_callback);

    rg.initScene();
    rg.generateQuads();

    while(!rg.updateScene())	// Check if the ESC key was pressed or the window was closed
    {
       usleep(100);
       rg.render();
    }

    rg.close();

    return 0;
}
