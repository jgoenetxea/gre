#include "videoBackgroundHandler.h"
namespace gre
{

static VideoBackgroundHandler* _instance = nullptr;

VideoBackgroundHandler* VideoBackgroundHandler::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new VideoBackgroundHandler();
    }

    return _instance;
}

VideoBackgroundHandler::VideoBackgroundHandler() : m_renderer(nullptr)
{
    initialize();
}

VideoBackgroundHandler::~VideoBackgroundHandler()
{
    delete(_instance);
}


void VideoBackgroundHandler::setFrame( const unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int channels )
{
    m_vp.setFrame( data, width, height, channels );
}

void VideoBackgroundHandler::show()
{
    m_renderer->renderScene(&m_scene);
}


void VideoBackgroundHandler::initialize()
{
    // get renderer instance
    m_renderer = gre::Renderer::getInstance();

#ifndef ORTHO_CAMERA
    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 500 );
    glm::vec3 up = glm::vec3( 0,1,0 );
    float fov = 60.0;

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_camera.setConfiguration(fov*GRAD2RAD, 4.0f / 3.0f, 0.1f, 1000.0f);
    // View matrix
    m_camera.setLocation( position,         // Camera is here
                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                          up                // Head is up (set to 0,-1,0 to look upside-down)
                          );
#else
    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    glm::vec3 up = glm::vec3( 0,1,0 );

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_camera.setConfiguration(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
    // View matrix
    m_camera.setLocation( position,           // Camera is here
                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                          up                  // Head is up (set to 0,-1,0 to look upside-down)
                          );
#endif

    // Generate scene
    m_scene.addCamera(m_camera);
    m_scene.addChild(&m_vp);

    m_vp.setName("BackgroundVideoPanel");
}

void VideoBackgroundHandler::fromScreenToImageCoordinates(float& x, float& y)
{
    // Generate the projection matrix
    m_perspectiveMatrix = m_scene.getCurrentCamera()->getProjectionMatrix();
    m_viewMatrix = m_scene.getCurrentCamera()->getViewMatrix();
    m_modelMatrix = m_vp.getGlobalMatrix();

    glm::mat4 projectionMatrix = m_perspectiveMatrix * m_viewMatrix * m_modelMatrix;
    glm::mat4 projectionMatrixInverse = glm::inverse(projectionMatrix);

    // Apply the projection matrix
    glm::vec4 screenPoint(x,y,0,1);
    glm::vec4 modelPoint = projectionMatrixInverse * screenPoint;

    x = modelPoint.x;
    y = modelPoint.y;
}

}
