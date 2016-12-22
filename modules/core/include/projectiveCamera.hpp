#ifndef PROJECTIVECAMERA_H
#define PROJECTIVECAMERA_H

#include "camera.hpp"

#include <glm/glm.hpp>
namespace gre
{

class ProjectiveCamera : public Camera
{
public:
    ProjectiveCamera();
    ~ProjectiveCamera();

    void setConfiguration(const glm::mat4 &projMat);
    void setConfiguration(float fov, float asp, float near = 0.1f, float far = 100.0f);
    void setLocation( const glm::vec3& position,           // Camera is here
                      const glm::vec3& target, // and looks here : at the same position, plus "direction"
                      const glm::vec3& up);

};

}

#endif
