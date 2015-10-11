#include "customObj.hpp"

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>
//#include<GLES2/gl2.h>

#include "shaderProgram.hpp"
#include "texture.hpp"

using namespace std;
using namespace gre;

CustomObj::CustomObj()
{
    m_color.x = 128.f;
    m_color.y = 128.f;
    m_color.z = 128.f;
}

CustomObj::~CustomObj()
{
}

void CustomObj::setShaders( const string& vertexShaderCode, const string& fragmentShaderCode, const string& geometryShaderCode )
{
    //Obj::setShaders(vertexShaderCode, fragmentShaderCode, geometryShaderCode);
    m_currentVertexShaderCode = vertexShaderCode;
    m_currentFragmentShaderCode = fragmentShaderCode;
    m_currentGeometryShaderCode = geometryShaderCode;
    m_program = ProgramGenerator::makeProgram( vertexShaderCode, fragmentShaderCode, geometryShaderCode );
    m_matrixUniformLocator = glGetUniformLocation(m_program, "modelViewMatrix");
    m_textureUniformLocator  = glGetUniformLocation(m_program, "iChannel0");
    m_iResolutionUniformLocator = glGetUniformLocation(m_program, "iResolution");
    m_iGlobalTimeUniformLocator = glGetUniformLocation(m_program, "iGlobalTime");
    m_iMouseUniformLocator = glGetUniformLocation(m_program, "iMouse");
    m_iDateUniformLocator = glGetUniformLocation(m_program, "iDate");
    m_iSampleRateUniformLocator = glGetUniformLocation(m_program, "iSampleRate");

    m_colourUniformLocator = glGetUniformLocation(m_program, "iColour");
}

void CustomObj::setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::string& geometry_file_path )
{
    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertex_file_path.c_str(), std::ios::in);
    if(vertexShaderStream.is_open()){
        std::string line = "";
        while(getline(vertexShaderStream, line))
            vertexShaderCode += "\n" + line;
        vertexShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path.c_str());
        return ;
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
    if(fragmentShaderStream.is_open()){
        std::string line = "";
        while(getline(fragmentShaderStream, line))
            fragmentShaderCode += "\n" + line;
        fragmentShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragment_file_path.c_str());
        return ;
    }

    // Read the Fragment Shader code from the file
    std::string geometryShaderCode = "";
    if(static_cast<int>(geometry_file_path.size()) > 0)
    {
        std::ifstream geometryShaderStream(geometry_file_path.c_str(), std::ios::in);
        if(geometryShaderStream.is_open()){
            std::string line = "";
            while(getline(geometryShaderStream, line))
                geometryShaderCode += "\n" + line;
            geometryShaderStream.close();
        }
        else
        {
            printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", geometry_file_path.c_str());
            return ;
        }
    }

    return setShaders(vertexShaderCode, fragmentShaderCode, geometryShaderCode);
}

void CustomObj::updateFragmentShader( const string& fragmentShaderCode )
{
    glDeleteProgram( m_program );
    setShaders( m_currentVertexShaderCode, fragmentShaderCode, m_currentGeometryShaderCode );
}

void CustomObj::draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective )
{
    /*glUseProgram( m_program );

    glUniform3i( m_colourUniformLocator, 255, 0, 0);

    Obj::draw(model, view, perspective);*/

    glUseProgram( m_program );

    // Compute model view perspective matrix
    m_mvp = perspective * view * model;

    // Compute elapsed time
    if( m_timer == NULL )
    {
        m_timer = new gre::Timer();
        m_timer->init();
    }
    m_elapsedTime = static_cast<float>(m_timer->getElapsedSeconds());

    // Get viewport size (yes, is dirty, but is not time for fancy solutions
    glGetIntegerv( GL_VIEWPORT, m_viewport );

    glUniformMatrix4fv( m_matrixUniformLocator, 1, GL_FALSE, &m_mvp[0][0] );
    glUniform1f( m_iGlobalTimeUniformLocator, m_elapsedTime );
    glUniform3f( m_iResolutionUniformLocator, m_viewport[2]-m_viewport[0], m_viewport[3]-m_viewport[1], 0.f);

    glUniform3f( m_colourUniformLocator, m_color.r, m_color.g, m_color.b);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(m_textureUniformLocator, 0);

    // Initialize the buffers if they are not initialized jet
    if( m_vertexBuffer == 0 )
    {
        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
    }
    if( m_uvBuffer == 0 )
    {
        glGenBuffers(1, &m_uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
    }
    if( m_normalBuffer == 0 )
    {
        glGenBuffers(1, &m_normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
    }

    // Store attribute values in their buffers
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    if( m_uvBuffer != 0 )
    {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
    }

    // 3rd attribute buffer : Normals
    if( m_normalBuffer != 0 )
    {
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
        glVertexAttribPointer(
            2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size :
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
    }

    //glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    for( int mesh=0, nMesh=m_meshList.size() ; mesh<nMesh ; ++mesh )
    {
        m_meshList[ mesh ]->draw();
    }

    glDisableVertexAttribArray(0);
    if( m_uvBuffer != 0 ) 	 glDisableVertexAttribArray(1);
    if( m_normalBuffer != 0 ) glDisableVertexAttribArray(2);
}

void CustomObj::initWithObj(Obj& obj) // move assignment
{
    // TODO: Hacer que esto sea configurable para cada obj
    m_mvp = obj.m_mvp;

    m_vertices = obj.m_vertices;
    m_uvs = obj.m_uvs;
    m_normals = obj.m_normals;

    // List of meshes in the object
    m_meshList = obj.m_meshList;

    m_currentFragmentShaderCode = getCurrentFragmentShaderCode();
    m_currentVertexShaderCode = getCurrentVertexShaderCode();
    m_currentGeometryShaderCode = getCurrentVertexShaderCode();

    this->setShaders( m_currentVertexShaderCode, m_currentFragmentShaderCode, m_currentGeometryShaderCode );
}
