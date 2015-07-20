#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    setUp( glm::vec3(0,1,0) );
}

Camera::~Camera()
{

}

void Camera::setConfiguration(float fov, float asp, float near, float far)
{
    m_projectionMatrix = glm::perspective(fov, asp, near, far);
}

void Camera::setLocation( const glm::vec3& position,           // Camera is here
                          const glm::vec3& target, // and looks here : at the same position, plus "direction"
                          const glm::vec3& up)
{
    m_up = up;
    m_viewMatrix = glm::lookAt( position, target, up );
}

void Camera::setUp( const glm::vec3& up )
{
    m_up = up;
}
