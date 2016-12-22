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

    void setConfiguration(const float userXOffset, const float userYOffset,
                          const float userZOffset, const float screenSize,
                          const float asp, const float near = 0.1f,
                          const float far = 100.0f);

    void setLocation( const glm::vec3& position,
                      const float angle,
                      const glm::vec3& axis );

private:
    glm::vec3 m_position;
    glm::vec3 m_axis;
    float m_angle;

    void computeViewMatrix( const glm::vec3& position,
                            const float angle,
                            const glm::vec3& axis );
};

}

#endif
