#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace gre;

Camera::Camera()
{
    setUp( glm::vec3(0,1,0) );
}

Camera::~Camera()
{

}

void Camera::setUp( const glm::vec3& up )
{
    m_up = up;
}

void Camera::setViewport(const int x, const int y,
                         const int width, const int height) {
    m_viewport = glm::vec4(x, y, width, height);
}
