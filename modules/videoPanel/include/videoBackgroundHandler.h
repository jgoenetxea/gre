#ifndef VIDEOBACKGROUNDHANDLER_H
#define VIDEOBACKGROUNDHANDLER_H

#include "shapes.hpp"

#include "scene.hpp"

#include "renderer.hpp"
#include "projectiveCamera.hpp"
#include "orthoCamera.hpp"

#include "videoPanel.hpp"

#define ORTHO_CAMERA true

namespace gre
{

enum ImageAlignment{
    GRE_HORIZONTAL_ALIGN_IMAGE_CENTER,
    GRE_ALIGN_IMAGE_LEFT,
    GRE_ALIGN_IMAGE_RIGHT,
};

class VideoBackgroundHandler
{
public:
    static VideoBackgroundHandler* getInstance();
    ~VideoBackgroundHandler();

    void show();

    void setFrame( const unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int channels );

    void fromScreenToImageCoordinates(float& x, float& y);

private:

    VideoBackgroundHandler();

    void initialize();

    gre::Scene m_scene;
    gre::Renderer* m_renderer;
    gre::VideoPanel m_vp;

    glm::mat4 m_perspectiveMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_modelMatrix;

#ifndef ORTHO_CAMERA
    gre::ProjectiveCamera m_camera;
#else
    gre::OrthoCamera m_camera;
#endif
};

}

#endif // VIDEOBACKGROUNDHANDLER_H
