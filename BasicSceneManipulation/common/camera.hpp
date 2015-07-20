#ifndef CAMERA_H
#define CAMERA_H

#include "node.hpp"

#include <glm/glm.hpp>

class Camera : public Node
{
public:
    Camera();
    ~Camera();

    void setConfiguration(float fov, float asp, float near = 0.1f, float far = 100.0f);
    void setLocation( const glm::vec3& position,           // Camera is here
                      const glm::vec3& target, // and looks here : at the same position, plus "direction"
                      const glm::vec3& up);
    void setUp( const glm::vec3& up );

private:
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 m_projectionMatrix;
    // Camera matrix
    glm::mat4 m_viewMatrix;

    glm::vec3 m_up;
};

#endif
