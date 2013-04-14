/*
 * CommonWall.cpp

 *
 *  Created on: Apr 8, 2013
 *      Author: goe
 */
#include <GL/glew.h>

#include "CommonWall.h"
#include "common/shapes.hpp"
#include "common/SceneControl.hpp"
#include "common/texture.hpp"

#include "common/objloader.hpp"

using namespace std;

CommonWall::CommonWall()
{
	// TODO Auto-generated constructor stub
	m_position = glm::vec3(0,0,0);
	m_orientation = 0;
	m_height = 1;
	m_width = 1;
	// texturing
	m_commonTexture = 0;

	m_shapeBuilder = new Shapes();

	m_sceneControl = SceneControl::getInstance();

	m_wallModel = NULL;
}

CommonWall::~CommonWall()
{
	// TODO Auto-generated destructor stub
}

void CommonWall::drawCube()
{
	glm::mat4 ProjectionMatrix = m_sceneControl->getProjectionMatrix();
	glm::mat4 ViewMatrix = m_sceneControl->getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Use our shader
	glUseProgram( m_sceneControl->getProgram() );

	glUniformMatrix4fv( m_sceneControl->getMatrixID(), 1, GL_FALSE, &MVP[0][0] );

	// TODO: change the cube drawing with a obj model as walls
	// set the model matrix
	m_shapeBuilder->drawCube(m_sceneControl->getTexture(), m_sceneControl->getTextureID());
}

void CommonWall::drawObjModel()
{
	glm::mat4 ProjectionMatrix = m_sceneControl->getProjectionMatrix();
	glm::mat4 ViewMatrix = m_sceneControl->getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	m_wallModel->setMVP(MVP);
	m_wallModel->draw();
}

void CommonWall::setTexture( unsigned int txtId )
{
	m_commonTexture = txtId;
	if( m_wallModel != NULL )
	{
		m_wallModel->setTexture( txtId );
	}
}

void CommonWall::setObj( string filename, string vertexShaderFilename, string fragmentShaderFilename )
{
	// Load the obj file
	m_wallModel = ObjFactory::getInstance()->loadOBJ( filename );
	m_wallModel->setTexture( loadDDS( "uvtemplate.DDS" ) );
	m_wallModel->setShaders( vertexShaderFilename, fragmentShaderFilename );
}

unsigned int CommonWall::getTexture()
{
	return m_commonTexture;
}

void CommonWall::setPosition( glm::vec3 pos )
{
	m_position = pos;
}

glm::vec3 CommonWall::getPosition()
{
	return m_position;
}

void CommonWall::setBounds( float width, float height )
{
	m_width = width;
	m_height = height;
}

void CommonWall::getBounds( float* width, float* height )
{
	*width = m_width;
	*height = m_height;
}

void CommonWall::setOrientation( int ori )
{
	if( ori < 0 )
	{
		while( ori < 0 )
		{
			ori = 360 + ori;
		}
	}
	else if( ori > 360 )
	{
		while( ori > 360 )
		{
			ori = ori - 360;
		}
	}
	m_orientation = ori;
}

int CommonWall::getOrientation()
{
	return (int) m_orientation;
}
