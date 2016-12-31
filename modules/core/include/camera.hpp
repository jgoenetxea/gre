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

    virtual void setViewport(const int x, const int y,
                             const int width, const int height);

    glm::vec4 getViewport(){ return m_viewport; }

    glm::mat4& getProjectionMatrix(){ return m_projectionMatrix; }
    glm::mat4& getViewMatrix(){ return m_viewMatrix; }

    glm::vec3& getUp(){ return m_up; }
    glm::vec3& getLocation(){ return m_location; }
    glm::vec3& getTarget(){ return m_target; }

protected:
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 m_projectionMatrix;
    // Camera matrix
    glm::mat4 m_viewMatrix;

    glm::vec3 m_up;
    glm::vec3 m_location;
    glm::vec3 m_target;

    glm::vec4 m_viewport;
};

}

#endif
