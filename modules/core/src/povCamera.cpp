#include "povCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace gre;

POVCamera::POVCamera()
{
    m_projectionMatrix = glm::mat4(1.f);
    m_viewMatrix = glm::mat4(1.f);

    m_userOffset = glm::vec3(0, 0, 0);

    m_screenWidth = 1.f;
    m_screenHeight = 1.f;
    m_near = 0.1f;
    m_far = 100.f;
}

POVCamera::~POVCamera()
{

}


void POVCamera::setConfiguration(float screenWidth,
                                 float screenHeight,
                                 float near, float far)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_near = near;
    m_far = far;

    computeProjMatrix(m_userOffset, m_screenWidth, m_screenHeight,
                      m_near, m_far);
    computeViewMatrix(m_location, m_target, m_up);
}

void POVCamera::setCameraOffset(const float userXOffset,
                                const float userYOffset,
                                const float userZOffset) {
    setCameraOffset(glm::vec3(userXOffset, userYOffset, userZOffset));
}

void POVCamera::setCameraOffset(const glm::vec3& offset) {
    m_userOffset = glm::vec3(-offset.x, -offset.y, offset.z);

    // update the location of the camera.
    computeProjMatrix(m_userOffset, m_screenWidth, m_screenHeight,
                      m_near, m_far);
    computeViewMatrix(m_location, m_target, m_up);
}

void POVCamera::setLocation(const glm::vec3& position,           // Camera is here
                            const glm::vec3& target, // and looks here : at the same position, plus "direction"
                            const glm::vec3& up)
{
    m_location = position;
    m_target = target;
    m_up = up;
    computeViewMatrix(m_location, m_target, m_up);
}

void POVCamera::computeViewMatrix(const glm::vec3& position,
                                  const glm::vec3& target,
                                  const glm::vec3& up) {
    glm::vec3 r;  // Horizontal movement
    glm::vec3 dir = target - position;
    dir = glm::normalize(dir);
    // Find the movement plane
    r = glm::cross(dir, up);
    // Normalize the r vector
    r = glm::normalize(r);
    // Resize the vector using the desplacement of the user in x
    r.x *= m_userOffset.x;
    r.y *= m_userOffset.x;
    r.z *= m_userOffset.x;
    // Compute the position of the user in the vertical axis
    glm::vec3 s;  // Vertical movement
    s.x = up.x * m_userOffset.y;
    s.y = up.y * m_userOffset.y;
    s.z = up.z * m_userOffset.y;
    // Compute the user position in the distance axis
    glm::vec3 z;  // User-screen distance movement
    z.x = dir.x * m_userOffset.z;
    z.y = dir.y * m_userOffset.z;
    z.z = dir.z * m_userOffset.z;
    // Compute position and orientation values
    glm::vec3 newPos(position.x - r.x - s.x - z.x,
                     position.y - r.y - s.y - z.y,
                     position.z - r.z - s.z - z.z);
    glm::vec3 newTar = newPos + dir;
    // Compute the view matrix for the camera
    m_viewMatrix = glm::lookAt(newPos, newTar, up);
}

void POVCamera::computeProjMatrix(const glm::vec3 camOffset,
                                  const float screenWidth,
                                  const float screenHeight,
                                  const float near,
                                  const float far) {
    float near_proPlane = near;  // fLength / 5;
    float ratio  = screenWidth / screenHeight;
    float wd2    = near_proPlane * screenHeight/(camOffset.z * 2.f);
    float ndfl   = near_proPlane / camOffset.z;
    // --- //
    float left   = - ratio * wd2 + camOffset.x * ndfl;
    float right  =   ratio * wd2 + camOffset.x * ndfl;
    float top    =   wd2 + camOffset.y * ndfl;
    float bottom = - wd2 + camOffset.y * ndfl;

    m_projectionMatrix = glm::frustum(left, right, bottom, top, near, far);

    //m_projectionMatrix = glm::perspective(45.f*3.1415f/180.f, asp, 0.1f, 10000.0f);
}
