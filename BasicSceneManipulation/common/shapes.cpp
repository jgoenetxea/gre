/*
 * Shapes.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: goe
 */

#include "shapes.hpp"

// Include GLEW
#include <GL/glew.h>

Shapes::Shapes()
{
	// TODO Auto-generated constructor stub
	m_cubeVertexBuffer = 0;
	m_cubeuvBuffer = 0;
}

Shapes::~Shapes()
{
	// TODO Auto-generated destructor stub
}

Obj* Shapes::getCube()
{
    Obj* obj = new Obj();

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uv;

    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f,-1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f,-1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    temp_vertices.push_back(glm::vec3( 1.0f,-1.0f, 1.0f));

    temp_uv.push_back(glm::vec2(0.000059f, 0.000004f));
    temp_uv.push_back(glm::vec2(0.000103f, 0.336048f));
    temp_uv.push_back(glm::vec2(0.335973f, 0.335903f));
    temp_uv.push_back(glm::vec2(1.000023f, 0.000013f));
    temp_uv.push_back(glm::vec2(0.667979f, 0.335851f));
    temp_uv.push_back(glm::vec2(0.999958f, 0.336064f));
    temp_uv.push_back(glm::vec2(0.667979f, 0.335851f));
    temp_uv.push_back(glm::vec2(0.336024f, 0.671877f));
    temp_uv.push_back(glm::vec2(0.667969f, 0.671889f));
    temp_uv.push_back(glm::vec2(1.000023f, 0.000013f));
    temp_uv.push_back(glm::vec2(0.668104f, 0.000013f));
    temp_uv.push_back(glm::vec2(0.667979f, 0.335851f));
    temp_uv.push_back(glm::vec2(0.000059f, 0.000004f));
    temp_uv.push_back(glm::vec2(0.335973f, 0.335903f));
    temp_uv.push_back(glm::vec2(0.336098f, 0.000071f));
    temp_uv.push_back(glm::vec2(0.667979f, 0.335851f));
    temp_uv.push_back(glm::vec2(0.335973f, 0.335903f));
    temp_uv.push_back(glm::vec2(0.336024f, 0.671877f));
    temp_uv.push_back(glm::vec2(1.000004f, 0.671847f));
    temp_uv.push_back(glm::vec2(0.999958f, 0.336064f));
    temp_uv.push_back(glm::vec2(0.667979f, 0.335851f));
    temp_uv.push_back(glm::vec2(0.668104f, 0.000013f));
    temp_uv.push_back(glm::vec2(0.335973f, 0.335903f));
    temp_uv.push_back(glm::vec2(0.667979f, 0.335851f));
    temp_uv.push_back(glm::vec2(0.335973f, 0.335903f));
    temp_uv.push_back(glm::vec2(0.668104f, 0.000013f));
    temp_uv.push_back(glm::vec2(0.336098f, 0.000071f));
    temp_uv.push_back(glm::vec2(0.000103f, 0.336048f));
    temp_uv.push_back(glm::vec2(0.000004f, 0.671870f));
    temp_uv.push_back(glm::vec2(0.336024f, 0.671877f));
    temp_uv.push_back(glm::vec2(0.000103f, 0.336048f));
    temp_uv.push_back(glm::vec2(0.336024f, 0.671877f));
    temp_uv.push_back(glm::vec2(0.335973f, 0.335903f));
    temp_uv.push_back(glm::vec2(0.667969f, 0.671889f));
    temp_uv.push_back(glm::vec2(1.000004f, 0.671847f));
    temp_uv.push_back(glm::vec2(0.667979f, 0.335851f));

    obj->m_vertices = temp_vertices;
    obj->m_uvs = temp_uv;

    return obj;
}

void Shapes::drawCube( unsigned int textureFileRef, unsigned int textureUniformLocator )
{
	if( m_cubeVertexBuffer == 0 )
	{
		glGenBuffers(1, &m_cubeVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_cubeVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SHAPE_CUBE_VERTEX), SHAPE_CUBE_VERTEX, GL_STATIC_DRAW);
	}

	if( m_cubeuvBuffer == 0 )
	{
		glGenBuffers(1, &m_cubeuvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_cubeuvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SHAPE_CUBE_UV), SHAPE_CUBE_UV, GL_STATIC_DRAW);
	}

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureFileRef);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(textureUniformLocator, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeuvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

