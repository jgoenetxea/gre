#ifndef CAMERA_H
#define CAMERA_H

#include "node.hpp"

#include <glm/glm.hpp>

namespace gre
{

class Camera : public Node
{
public:
    Camera();
    ~Camera();

    virtual void setUp( const glm::vec3& up );

    glm::mat4& getProjectionMatrix(){ return m_projectionMatrix; }
    glm::mat4& getViewMatrix(){ return m_viewMatrix; }

protected:
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 m_projectionMatrix;
    // Camera matrix
    glm::mat4 m_viewMatrix;

    glm::vec3 m_up;
};

}

#endif
