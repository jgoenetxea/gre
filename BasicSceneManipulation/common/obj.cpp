#include "obj.hpp"

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "shaderProgram.hpp"
#include "texture.hpp"

using namespace std;

Obj::Obj()
{
	m_texture = 0;
	m_textureUniformLocator = 0;
	m_matrixUniformLocator = 0;
	m_program = 0;

	m_vertexBuffer = 0;
	m_uvBuffer = 0;
	m_normalBuffer = 0;
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

void Obj::setTexture( unsigned int textureFileRef )
{
	m_texture = textureFileRef;
}

void Obj::setTexture( std::string& textureFileName )
{
    m_texture = loadDDS(textureFileName.c_str());
}

void Obj::setShaders( string vertexShaderFilename, string fragmentShaderFilename )
{
    m_program = ProgramGenerator::makeProgramUsingFiles( vertexShaderFilename, fragmentShaderFilename );
    m_matrixUniformLocator = glGetUniformLocation(m_program, "MVP");
    m_textureUniformLocator  = glGetUniformLocation(m_program, "uSampler");
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

void Obj::draw( glm::mat4& model, glm::mat4& view, glm::mat4& perspective )
{
	glUseProgram( m_program );

    m_mvp = perspective * view * model;

	glUniformMatrix4fv( m_matrixUniformLocator, 1, GL_FALSE, &m_mvp[0][0] );

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

void Obj::buildObject()
{
	//For each
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