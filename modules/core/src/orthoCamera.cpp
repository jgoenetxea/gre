#include "orthoCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "logger.h"
#define LOG_TAG "ORTHO-CAMERA"

using namespace gre;

OrthoCamera::OrthoCamera()
{

}

OrthoCamera::~OrthoCamera()
{

}

void OrthoCamera::setConfiguration(float left, float right, float bottom, float top, float near, float far)
{
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    m_near = near;
    m_far = far;
    m_projectionMatrix = glm::ortho(
            left,
            right,
            bottom,
            top,
            near,
            far
        );
}

void OrthoCamera::setConfiguration(float refSizeValue, float near, float far, CameraConfigPrior refDim)
{
    int viewportWidth, viewportHeight;
    float cameraWidth, cameraHeight;

    getViewportSize(viewportWidth, viewportHeight);
    float aspectRatio = static_cast<float>(viewportHeight)/static_cast<float>(viewportWidth);

    if(refDim == GRE_USE_WIDTH)
    {
        cameraWidth = refSizeValue;
        cameraHeight = cameraWidth * aspectRatio;
    }
    else if(refDim == GRE_USE_HEIGHT)
    {
        cameraHeight = refSizeValue;
        cameraWidth = cameraWidth / aspectRatio;
    }
    else
    {
        LOGE("Error: The definition fo the camera is wrong\n");
        return ;
    }

    m_left = -cameraWidth;
    m_right = cameraWidth;
    m_bottom = -cameraHeight;
    m_top = cameraHeight;
    m_near = near;
    m_far = far;
    m_projectionMatrix = glm::ortho(
            m_left,
            m_right,
            m_bottom,
            m_top,
            m_near,
            m_far
        );
}

void OrthoCamera::setLocation( const glm::vec3& position,           // Camera is here
                          const glm::vec3& target, // and looks here : at the same position, plus "direction"
                          const glm::vec3& up)
{
    m_up = up;
    m_location = position;
    m_target = target;
    m_viewMatrix = glm::lookAt( position, target, up );
}
