#ifndef OBJ_H
#define OBJ_H

#include "transformation.hpp"
#include "timer.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>

using namespace std;

namespace gre
{

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

class Obj: public Transformation
{
public:
	Obj();
	~Obj();

    // TODO: Hacer que esto sea configurable para cada obj
    glm::mat4 m_mvp;

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_uvs;
    std::vector<glm::vec3> m_normals;

    // List of meshes in the object
    std::vector<Mesh*> m_meshList;

    void draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective );
    void setTexture( unsigned int textureFileRef );
    void setTexture( string &textureFileName );

    void setShaders( const string& vertexShaderCode, const string& fragmentShaderCode, const string& geometryShaderCode, const std::vector<std::string>& extraValues );

    void setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::vector<std::string>& extraValues );
    void setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::string& geometry_file_path="" );
    void setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::string& geometry_file_path, const std::vector<std::string>& extraValues );

    void updateFragmentShader( const string& fragmentShaderCode );
    void updateFragmentShader( const string& fragmentShaderCode, const std::vector<std::string>& extraValues );

    void getInnerData( std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals  );
    void setInnerData( const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals  );

    std::vector<Mesh*>& getMeshList(){ return m_meshList; }
    void setMeshList( const std::vector<Mesh*>& meshList );

    std::string& getCurrentFragmentShaderCode(){ return m_currentFragmentShaderCode; }
    std::string& getCurrentVertexShaderCode(){ return m_currentVertexShaderCode; }
    std::string& getCurrentGeometryShaderCode(){ return m_currentGeometryShaderCode; }

    void setExtraValues( const std::vector<std::vector<float> >& extraElementValues );


protected:
	unsigned int m_vertexBuffer, m_uvBuffer, m_normalBuffer;
	unsigned int m_texture;
    unsigned int m_textureUniformLocator;
    unsigned int m_matrixUniformLocator;
    unsigned int m_iResolutionUniformLocator;
    unsigned int m_iGlobalTimeUniformLocator;
    unsigned int m_iMouseUniformLocator;
    unsigned int m_iDateUniformLocator;
    unsigned int m_iSampleRateUniformLocator;
    unsigned int m_program;

    float m_elapsedTime;    // Elapsed time in seconds
    Timer* m_timer;
    int m_viewport[4];

    string m_currentVertexShaderCode;
    string m_currentFragmentShaderCode;
    string m_currentGeometryShaderCode;

    bool m_extraValuesDefined;
    std::vector<std::string> m_extraValueNames;
    std::vector<unsigned int> m_extraUniforms;
    std::vector<std::vector<float> > m_extraUniformValues;

	void buildObject();

    void applyExtraValues( const unsigned int& program, const std::vector<unsigned int>& extraElementIds, const std::vector<std::vector<float> >& extraElementValues );
};

}

#endif
