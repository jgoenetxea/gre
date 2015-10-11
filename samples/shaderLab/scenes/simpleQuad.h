#ifndef SIMPLEQUAD_H
#define SIMPLEQUAD_H

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>
//#include<GLES2/gl2.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "objloader.hpp"
#include "shapes.hpp"

#include "scene.hpp"
#include "translation.hpp"

#include "renderer.hpp"
#include "orthoCamera.hpp"

#include "timer.h"

class SimpleQuad
{

public:
    SimpleQuad();
    ~SimpleQuad();

    void initialize();
    void paint();

    void updateFragmentShader( const string& fragmentShaderCode );

    std::string& getCurrentFragmentShaderCode();
    std::string& getCurrentVertexShaderCode();

private:
    GLuint VertexArrayID;

    gre::Scene m_scene;
    gre::Renderer* m_renderer;
    gre::Obj* m_obj;
    gre::Translation m_trans;

    gre::OrthoCamera m_camera;
    glm::vec3 position;
    glm::vec3 up;

    gre::Timer* m_timer;
    float horizontalAngle;
    float translateValue;
    int rotSpeed;
    int transSpeed;

    // Asstes
    std::string m_assets_path;
    std::string m_vShader;
    std::string m_fShader;
    std::string m_fColourShader;
    std::string m_uvtemplate;
    std::string m_modelFile;

    int m_frame;
};


#endif
