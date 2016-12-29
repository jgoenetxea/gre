#ifndef POVCAMERA_H
#define POVCAMERA_H

#include "camera.hpp"

#include <glm/glm.hpp>
namespace gre
{

class POVCamera : public Camera
{
public:
    POVCamera();
    ~POVCamera();

    void setConfiguration(float screenWidth,
                          float screenHeight,
                          float near = 0.1f, float far = 100.0f);

    void setCameraOffset(const glm::vec3& offset);

    void setCameraOffset(const float userXOffset,
                         const float userYOffset,
                         const float userZOffset);

    void setLocation( const glm::vec3& position,           // Camera is here
                      const glm::vec3& target, // and looks here : at the same position, plus "direction"
                      const glm::vec3& up);

private:
    // The focal distance defines the distance between the camera and the
    // screen plane.
    float m_screenWidth;
    float m_screenHeight;
    float m_near;
    float m_far;

    glm::vec3 m_userOffset;

    void computeViewMatrix(const glm::vec3& position,
                           const glm::vec3& target,
                           const glm::vec3& up);

    void computeProjMatrix(const glm::vec3 camOffset,
                           const float screenWidth,
                           const float screenHeight,
                           const float near,
                           const float far);
};

}

#endif
