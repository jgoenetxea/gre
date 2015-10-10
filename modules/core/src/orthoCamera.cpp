#include "orthoCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

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

void OrthoCamera::setLocation( const glm::vec3& position,           // Camera is here
                          const glm::vec3& target, // and looks here : at the same position, plus "direction"
                          const glm::vec3& up)
{
    m_up = up;
    m_location = position;
    m_target = target;
    m_viewMatrix = glm::lookAt( position, target, up );
}

