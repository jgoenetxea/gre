#include "obj.hpp"

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

Obj::Obj() :
    m_timer(NULL),
    m_texture(0),
    m_textureUniformLocator(0),
    m_matrixUniformLocator(0),
    m_program(0),
    m_vertexBuffer(0),
    m_uvBuffer(0),
    m_normalBuffer(0),
    m_currentVertexShaderCode(""),
    m_currentFragmentShaderCode(""),
    m_currentGeometryShaderCode(""),
    m_extraValuesDefined(false)
{

}

Obj::~Obj()
{
	if( m_meshList.size() > 0 )
	{
		for(int i=0, n=m_meshList.size() ; i<n ; ++i)
		{
			delete m_meshList[i];
		}
		m_meshList.clear();
	}
}

void Obj::setTexture( const unsigned int textureFileRef )
{
	m_texture = textureFileRef;
}

void Obj::setTexture( const std::string& textureFileName )
{
    int dotPos = textureFileName.rfind('.');
    if(dotPos != std::string::npos)
    {
        std::string extension = textureFileName.substr(dotPos+1, static_cast<int>(textureFileName.size())-dotPos);
        std::locale loc;
        std::stringstream ss;
        for (std::string::size_type i=0; i<extension.length(); ++i)
            ss << std::tolower(extension[i],loc);

        extension = ss.str();

        if(extension == "png")
        {
            m_texture = Texture::loadPNG(textureFileName.c_str());
        }
        else if(extension == "dds")
        {
            m_texture = Texture::loadDDS(textureFileName.c_str());
        }
    }
}

void Obj::setShaders( const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::string& geometryShaderCode, const std::vector<std::string>& extraValues )
{
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

    if(extraValues.size() > (unsigned int)0)
    {
        m_extraValueNames = extraValues;
        m_extraValuesDefined = true;
        m_extraUniforms.clear();
        GLint value;
        for(std::vector<std::string>::const_iterator it = extraValues.begin() ; it != extraValues.end() ; ++it)
        {
            value = glGetUniformLocation(m_program, (*it).c_str());
            m_extraUniforms.push_back(value);
        }
    }
    else
    {
        m_extraValuesDefined = false;
    }
}

void Obj::setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::vector<std::string>& extraValues )
{
    setShadersFromFiles(vertex_file_path, fragment_file_path, "", extraValues);
}

void Obj::setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::string& geometry_file_path )
{
    std::vector<std::string> extraValues;
    setShadersFromFiles(vertex_file_path, fragment_file_path, geometry_file_path, extraValues);
}

void Obj::setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::string& geometry_file_path, const std::vector<std::string>& extraValues )
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

    // Read the Geometry Shader code from the file
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

    return setShaders(vertexShaderCode, fragmentShaderCode, geometryShaderCode, extraValues);
}

void Obj::updateFragmentShader( const std::string& fragmentShaderCode )
{
    glDeleteProgram( m_program );
    setShaders( m_currentVertexShaderCode, fragmentShaderCode, m_currentGeometryShaderCode, m_extraValueNames );
}

void Obj::updateFragmentShader( const std::string& fragmentShaderCode, const std::vector<std::string>& extraValues )
{
    glDeleteProgram( m_program );
    setShaders( m_currentVertexShaderCode, fragmentShaderCode, m_currentGeometryShaderCode, extraValues );
}

void Obj::setExtraValues( const std::vector<std::vector<float> >& extraElementValues )
{
    m_extraUniformValues = extraElementValues;
}

void Obj::applyExtraValues( const unsigned int& program, const std::vector<unsigned int>& extraElementIds, const std::vector<std::vector<float> >& extraElementValues )
{
    unsigned int n = extraElementIds.size();
    unsigned int uniformId = 0;
    for(int element=0 ; element<n ; ++element)
    {
        unsigned int size = extraElementValues[element].size();
        uniformId = extraElementIds[element];
        switch(size)
        {
            case 1:{
                glUniform1f( uniformId, extraElementValues[element][0] );
                break;
            }
            case 2:{
                glUniform2f( uniformId, extraElementValues[element][0], extraElementValues[element][1] );
                break;
            }
            case 3:{
                glUniform3f( uniformId, extraElementValues[element][0], extraElementValues[element][1], extraElementValues[element][2] );
                break;
            }
            case 4:{
                glUniform4f( uniformId, extraElementValues[element][0], extraElementValues[element][1], extraElementValues[element][2], extraElementValues[element][3] );
                break;
            }
            default:{
                printf("Uniform size not managed!!\n");
            }

        }
    }
}

//void Obj::setMVP( glm::mat4& MVP )
//{
//	m_mvp = MVP;
//}

//void Obj::setMVP( glm::mat4& model, glm::mat4& view, glm::mat4& perspective )
//{
//    m_mvp = perspective * view * model;
//}

//void Obj::setVP( glm::mat4& view, glm::mat4& perspective )
//{
//    m_mvp = perspective * view * m_globalTranslation;
//}

void Obj::draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective )
{
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

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(m_textureUniformLocator, 0);

    if(m_extraValuesDefined)
    {
        applyExtraValues(m_program, m_extraUniforms, m_extraUniformValues);
    }

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

void Obj::buildObject()
{
	//For each
}

void Obj::getInnerData( std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals  )
{
    vertices = m_vertices;
    uvs = m_uvs;
    normals = m_normals;
}

void Obj::setInnerData( const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals  )
{
    m_vertices = vertices;
    m_uvs = uvs;
    m_normals = normals;

    if( m_vertexBuffer != 0 ) glDeleteBuffers(1, &m_vertexBuffer);
    if( m_uvBuffer != 0 ) glDeleteBuffers(1, &m_uvBuffer);
    if( m_normalBuffer != 0 ) glDeleteBuffers(1, &m_normalBuffer);

    m_vertexBuffer = 0;
    m_uvBuffer = 0;
    m_normalBuffer = 0;
}

void Obj::setMeshList( const std::vector<Mesh*>& meshList )
{
    if( m_meshList.size() > 0 ) m_meshList.clear();

    m_meshList = meshList;
}

/******************************************************************************/
/****  					Mesh object									***********/
/******************************************************************************/

Mesh::Mesh()
{
	m_indexBuffer = 0;
	m_name = "";
}

Mesh::~Mesh(){}

void Mesh::draw()
{
	if( m_indexBuffer == 0 )
	{
		// Generate a buffer for the indices
    glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndex.size() * sizeof(unsigned short), &m_vertexIndex[0], GL_STATIC_DRAW);
	}

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	glDrawElements(	GL_TRIANGLES,      		 // mode
						m_vertexIndex.size(),    // count
						GL_UNSIGNED_SHORT,   		 // type
						(void*)0           		 // element array buffer offset
		     	 	 	);
}

void Mesh::setName( string& name )
{
	m_name = name;
}

//void Mesh::setShaders( string vertexShaderFilename, string fragmentShaderFilename )
//{
//    m_program = ProgramGenerator::makeProgramUsingFiles( vertexShaderFilename, fragmentShaderFilename );
//		m_matrixUniformLocator = glGetUniformLocation(m_program, "MVP");
//		m_textureUniformLocator  = glGetUniformLocation(m_program, "uSampler");
//}
