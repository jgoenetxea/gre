#include "rectGenerator.h"

#include "logger.h"
#define LOG_TAG "RectGenerator"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <typeinfo>

RectGenerator::RectGenerator()
{
    m_assets_path = ASSET_DIRECTORY;

    m_vShader = m_assets_path+"shaders/basic130.vert";
    m_fShader = m_assets_path+"shaders/basic130.frag";
    m_fColourShader = m_assets_path+"shaders/colour.frag";
    m_fGradientShader = m_assets_path+"shaders/gradient.frag";
    m_fWaveShader = m_assets_path+"shaders/glowedWave.frag";
    //m_uvtemplate = m_assets_path+"obj/uvtemplate.DDS";
    m_uvtemplate = m_assets_path+"images/test.png";
    m_modelFile = m_assets_path+"obj/cube.obj";
}

RectGenerator::~RectGenerator()
{

}

bool RectGenerator::initScene()
{
    LOGI("Initializing scene...");
    // get renderer instance
    m_renderer = gre::Renderer::getInstance();

    // Generate the main model
    m_base = gre::ShapeDispatcher::getShapes()->getQuad();
    m_base->setShadersFromFiles( m_vShader, m_fGradientShader );
    m_base->setTexture( m_uvtemplate );
    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    glm::vec3 up = glm::vec3( 0,1,0 );


    m_cube = static_cast<gre::CustomObj*>(gre::ShapeDispatcher::getShapes()->getQuad());
    m_cube->setShadersFromFiles( m_vShader, m_fWaveShader );
    m_cube->setTexture( m_uvtemplate );

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //m_camera.setConfiguration(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
    m_camera.setConfiguration(1.f, 0.1f, 100.0f);
    // View matrix
    m_camera.setLocation( position,           // Camera is here
            glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Generate scene
    m_scene.addCamera(m_camera);
    m_scene.addChild(m_base);
    m_scene.addChild(&m_trans);
    m_trans.addChild(m_cube);
    LOGI("Scene initialized!");

    // Locate the new quad
    m_trans.setScale(glm::vec3(0.2f, 0.2f, 0.2f));
    m_trans.setTranslation(glm::vec3(0.0f, 0.0f, 1.0f));

    // Init Rectangles
    m_maxNumberOfRectangles = 10;
    m_lowWidth = 1;
    m_highWidth = 5;
    m_lowHeight = 2;
    m_highHeight = 5;
    m_centerSquares = Point2D(0.0f, 0.0f);
    m_radiusSquares = 5.0f;
    LOGI("Scene configured!");

    return true;
}

bool RectGenerator::updateScene()
{
	static float position = 0.0f;
	m_trans.rotate(1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	position = position + 0.001f;

    return glfwWindowShouldClose(m_window);
}

bool RectGenerator::render()
{
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //scene.draw();
   m_renderer->renderScene(&m_scene);

   // Swap buffers
   glfwSwapBuffers(m_window);
   glfwPollEvents();
}

void RectGenerator::close()
{
    LOGI("Freeing scene memory..");
    for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
    {
        delete *it;
    }
    m_rectangles.clear();

    GLFWWindowImpl::close();
}

bool RectGenerator::generateQuads()
{
    LOGI("Generating scene...");

    // Create random squares
    for(int i = 0; i < m_maxNumberOfRectangles; i++)
    {
        float width = (float)math2d::randomNumberInterval(m_lowWidth, m_highWidth);
        float height = (float)math2d::randomNumberInterval(m_lowHeight, m_highHeight);
        Point2D origin = math2d::calculateRandomPoint2DInACircle(m_centerSquares, m_radiusSquares);
        Square2D* square = new Square2D(origin, width, height);
        m_rectangles.push_back(square);
    }

    // Print info
    for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
    {
        //std::string data = (*it)->getJSONFormat();
        //LOGD("Data: %s", data.c_str());
    }
    LOGI("Scene generated!");
    return true;
}

void RectGenerator::getCameraPosition(glm::vec3& pos)
{
    glm::mat4 transMat = m_camera.getGlobalTranslation();
}

void RectGenerator::setCameraPosition(glm::vec3& pos)
{
    glm::mat4 transMat = m_camera.getGlobalTranslation();
}

void RectGenerator::translateCameraPosition(const glm::vec3& offset)
{
    glm::vec3 up = m_camera.getUp();
    glm::vec3 location = m_camera.getLocation();
    glm::vec3 target = m_camera.getTarget();

    location += offset;
    target += offset;

    m_camera.setLocation(location, target, up);
}

void RectGenerator::zoom(const float value)
{
    m_camera.setConfiguration( -value,
                                value,
                               -value,
                                value,
                                m_camera.getNearValue(),
                                m_camera.getFarValue());
}
