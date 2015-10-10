#include "glfwWindow.h"

#include "objloader.hpp"
#include "shapes.hpp"
#include "scene.hpp"
#include "translation.hpp"
#include "renderer.hpp"
#include "projectiveCamera.hpp"
#include "orthoCamera.hpp"

#include "square2D.h"
#include "math2D.h"
using namespace threemonkeybits;

#include <string>

class RectGenerator : public GLFWWindow
{
public:
    RectGenerator();
    ~RectGenerator();

    bool initScene();

    virtual bool updateScene();

    virtual bool render();

    void close();

    bool generateQuads();

protected:
    // Asstes
    std::string m_assets_path;
    std::string m_vShader;
    std::string m_fShader;
    std::string m_uvtemplate;
    std::string m_modelFile;
    // Rendering elements
    gre::Renderer* m_renderer;
    gre::Obj* m_obj;
    gre::Translation m_trans;
    gre::Scene m_scene;
    gre::OrthoCamera m_camera;
    // Scene generated configuration
    int m_maxNumberOfRectangles;
    int m_lowWidth;
    int m_highWidth;
    int m_lowHeight;
    int m_highHeight;
    Point2D m_centerSquares;
    float m_radiusSquares;
    std::vector<Square2D*> m_rectangles;

};
