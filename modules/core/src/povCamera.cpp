#include "povCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace gre;

POVCamera::POVCamera()
{
    m_position = glm::vec3(0, 0, 0);
    m_axis = glm::vec3(0, 0, 0);
    m_viewMatrix = glm::mat4(1.f);
}

POVCamera::~POVCamera()
{

}

#define GRAD2RAD
void POVCamera::setConfiguration(const float userXOffset, const float userYOffset,
                                 const float userZOffset, const float screenSize,
                                 const float asp, const float near,
                                 const float far)
{
    float tanAlpha2 = (screenSize/2)/userZOffset;
    // float angle = 2 * arctg(tanAlpha2);
    float movIndex = near / userZOffset;
    float horiOffset = near / tanAlpha2;
    float left = -asp * horiOffset + (userXOffset * movIndex);
    float right = asp * horiOffset + (userXOffset * movIndex);
    float top =  horiOffset + (userYOffset * movIndex);
    float bottom = -horiOffset + (userYOffset * movIndex);

    m_projectionMatrix = glm::frustum(left, right, bottom, top, near, far);

    //m_projectionMatrix = glm::perspective(45.f*3.1415f/180.f, asp, 0.1f, 1000.0f);
}

void POVCamera::setLocation( const glm::vec3& position,
                             const float angle,
                             const glm::vec3& axis )
{
    m_position = position;
    m_angle = angle;
    m_axis = axis;

    computeViewMatrix(m_position, m_angle, m_axis);
}

void POVCamera::computeViewMatrix( const glm::vec3& position,
                             const float angle,
                             const glm::vec3& axis ) {
    glm::mat4 r = glm::rotate(glm::mat4(1.0), angle, axis);
    glm::mat4 t = glm::translate(glm::mat4(1.0), position);

    m_viewMatrix = r * t;
}
