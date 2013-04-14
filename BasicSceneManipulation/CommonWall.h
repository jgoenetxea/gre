/*
 * CommonWall.h
 *
 *  Created on: Apr 8, 2013
 *      Author: goe
 */

#ifndef COMMONWALL_H_
#define COMMONWALL_H_

#include <string>

#include <glm/glm.hpp>

class Shapes;
class SceneControl;
class Obj;

class CommonWall
{
public:
	CommonWall();
	virtual ~CommonWall();

	void setTexture( unsigned int txtId );
	unsigned int getTexture();

	void setPosition( glm::vec3 pos );
	glm::vec3 getPosition();

	void setBounds( float width, float height );
	void getBounds( float* width, float* height );

	void setOrientation( int ori );
	int getOrientation();

	void drawCube();

	void drawObjModel();
	void setObj( std::string filename, std::string vertexShaderFilename, std::string fragmentShaderFilename );

private:
	// location
	glm::vec3 m_position;
	unsigned int m_orientation;
	float m_height, m_width;
	// texturing
	unsigned int m_commonTexture;
	//std::vector<unsigned int> m_additionalTextures;

	// Other
	Shapes* m_shapeBuilder;
	SceneControl* m_sceneControl;

	// Obj model
	Obj* m_wallModel;
};

#endif /* COMMONWALL_H_ */
