#ifndef MOVINGCUBE_H
#define MOVINGCUBE_H

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "../../gre/objloader.hpp"
#include "../../gre/shapes.hpp"

#include "../../gre/scene.hpp"
#include "../../gre/translation.hpp"

#include "../../gre/renderer.hpp"
#include "../../gre/projectiveCamera.hpp"

#include "../../gre/timer.h"

class MovingCube
{

public:
    MovingCube();
    ~MovingCube();

    void initialize();
    void paint();

private:
    GLuint VertexArrayID;

    gre::Scene m_scene;
    gre::Renderer* m_renderer;
    gre::Obj* m_obj;
    gre::Translation m_trans;

    gre::ProjectiveCamera m_camera;
    glm::vec3 position;
    glm::vec3 up;

    gre::Timer* m_timer;
    float horizontalAngle;
    float translateValue;
    int rotSpeed;
    int transSpeed;

    int m_frame;
};


#endif
