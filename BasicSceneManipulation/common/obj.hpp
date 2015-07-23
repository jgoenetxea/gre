#ifndef OBJ_H
#define OBJ_H

#include "node.hpp"

#include <vector>
#include <string>

#include <glm/glm.hpp>

using namespace std;

class Mesh
{
public:
	Mesh();
	~Mesh();

    void draw();

	std::vector<unsigned short> m_vertexIndex;

	std::string m_name;

	void setName( std::string& name );

private:
	unsigned int m_indexBuffer;
};

class Obj: public Node
{
public:
	Obj();
	~Obj();

    void draw( glm::mat4& model, glm::mat4& view, glm::mat4& perspective );
    void setTexture( unsigned int textureFileRef );
    void setTexture( string &textureFileName );
	void setShaders( string vertexShaderFilename, string fragmentShaderFilename );

	// TODO: Hacer que esto sea configurable para cada obj
	glm::mat4 m_mvp;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;

	// List of meshes in the object
	std::vector<Mesh*> m_meshList;

private:
	unsigned int m_vertexBuffer, m_uvBuffer, m_normalBuffer;
	unsigned int m_texture;
	unsigned int m_textureUniformLocator, m_matrixUniformLocator;
	unsigned int m_program;

	void buildObject();
};

#endif
