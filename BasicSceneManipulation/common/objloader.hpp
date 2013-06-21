#ifndef OBJLOADER_H
#define OBJLOADER_H

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

class Obj
{
public:
	Obj();
	~Obj();

	void draw();
	void setTexture( unsigned int textureFileRef );
	void setShaders( string vertexShaderFilename, string fragmentShaderFilename );
	void setMVP( glm::mat4& MVP );

	// TODO: Hacer que esto sea configurable para cada obj
	glm::mat4 m_mvp;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;

	// List of meshes in the object
	std::vector<Mesh*> m_meshList;

private:
	unsigned int m_vertexBuffer, m_uvBuffer, m_normalBuffer;
	unsigned int m_texture, m_textureUniformLocator, m_matrixUniformLocator;
	unsigned int m_program;

	void buildObject();
};

class ObjFactory
{
public:
	ObjFactory();
	~ObjFactory();

	static ObjFactory* getInstance();
	static void deleteInstance();

	static Obj* loadOBJ( std::string filename );
	static int isEqualStored( 	std::vector<unsigned short>& vertexIndices,
									std::vector<unsigned short>& uvIndices,
									std::vector<unsigned short>& normalIndices,
									unsigned int currentIndex
									);
};

#endif
