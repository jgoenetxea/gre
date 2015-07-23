#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include "camera.hpp"

#include <glm/glm.hpp>
namespace gre
{

class OrthoCamera : public Camera
{
public:
    OrthoCamera();
    ~OrthoCamera();

    void setConfiguration(float left, float right, float bottom, float top, float near, float far);
    void setLocation( const glm::vec3& position,           // Camera is here
                      const glm::vec3& target, // and looks here : at the same position, plus "direction"
                      const glm::vec3& up);

};

}

#endif
