#include <transformation.hpp>

#include "glfwWindow.h"

#include "customObj.hpp"

#include "objloader.hpp"
#include "shapes.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "projectiveCamera.hpp"
#include "orthoCamera.hpp"

#include "square2D.h"
#include "math2D.h"
using namespace threemonkeybits;

#include <string>

class RectGenerator : public GLFWWindowImpl
{
public:
    RectGenerator();
    ~RectGenerator();

    bool initScene();

    virtual bool updateScene();

    virtual bool render();

    void close();

    // Create nodes objects
    bool generateQuads();
    void printQuadsInfo();

    // Create & destroy nodes in the scene from objects
    void createNodeQuads();
    void destroyNodeQuads();

    void getCameraPosition(glm::vec3& pos);
    void setCameraPosition(glm::vec3& pos);
    void translateCameraPosition(const glm::vec3& offset);
    void zoom(const float value);

protected:
    // Asstes
    std::string m_assets_path;
    std::string m_gShader;
    std::string m_vShader;
    std::string m_fShader;
    std::string m_fColourShader;
    std::string m_fWaveShader;
    std::string m_fGradientShader;
    std::string m_uvtemplate;
    std::string m_modelFile;

    std::string m_gShaderTron;
    std::string m_vShaderTron;
    std::string m_fShaderTron;

    // Rendering elements
    gre::Renderer* m_renderer;
    gre::Obj* m_base;
    gre::CustomObj* m_cube;
    gre::Scene m_scene;
    gre::OrthoCamera m_camera;

    // Scene generated configuration
    bool m_quadsGenerated;
    int m_maxNumberOfRectangles;
    int m_lowWidth;
    int m_highWidth;
    int m_lowHeight;
    int m_highHeight;
    Point2D m_centerSquares;
    float m_radiusSquares;
    std::vector<Square2D*> m_rectangles;

};
