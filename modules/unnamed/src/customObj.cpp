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

}

CustomObj::~CustomObj()
{
}

void CustomObj::setShaders( const string& vertexShaderCode, const string& fragmentShaderCode )
{
    m_currentVertexShaderCode = vertexShaderCode;
    m_currentFragmentShaderCode = fragmentShaderCode;
    m_program = ProgramGenerator::makeProgram( vertexShaderCode, fragmentShaderCode );
    m_matrixUniformLocator = glGetUniformLocation(m_program, "modelViewMatrix");
    m_textureUniformLocator  = glGetUniformLocation(m_program, "iChannel0");
    m_iResolutionUniformLocator = glGetUniformLocation(m_program, "iResolution");
    m_iGlobalTimeUniformLocator = glGetUniformLocation(m_program, "iGlobalTime");
    m_iMouseUniformLocator = glGetUniformLocation(m_program, "iMouse");
    m_iDateUniformLocator = glGetUniformLocation(m_program, "iDate");
    m_iSampleRateUniformLocator = glGetUniformLocation(m_program, "iSampleRate");
}

void CustomObj::setShadersFromFiles( std::string& vertex_file_path, std::string& fragment_file_path )
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
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path.c_str());
        return ;
    }

    return setShaders(vertexShaderCode, fragmentShaderCode);
}

void CustomObj::updateFragmentShader( const string& fragmentShaderCode )
{
    glDeleteProgram( m_program );
    setShaders( m_currentVertexShaderCode, fragmentShaderCode );
}

void CustomObj::draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective )
{
	glUseProgram( m_program );

    Obj::draw(model, view, perspective);
}

CustomObj& CustomObj::operator=(Obj& obj) // move assignment
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

    setShaders( m_currentVertexShaderCode, m_currentFragmentShaderCode );

    return *this;
}
