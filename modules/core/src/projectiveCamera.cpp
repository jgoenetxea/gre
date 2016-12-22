#include "projectiveCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace gre;

ProjectiveCamera::ProjectiveCamera()
{

}

ProjectiveCamera::~ProjectiveCamera()
{

}

void ProjectiveCamera::setConfiguration(const glm::mat4 &projMat)
{
    m_projectionMatrix = projMat;
}

void ProjectiveCamera::setConfiguration(float fov, float asp, float near, float far)
{
    m_projectionMatrix = glm::perspective(fov, asp, near, far);
}

void ProjectiveCamera::setLocation( const glm::vec3& position,           // Camera is here
                          const glm::vec3& target, // and looks here : at the same position, plus "direction"
                          const glm::vec3& up)
{
    m_up = up;
    m_viewMatrix = glm::lookAt( position, target, up );
}
