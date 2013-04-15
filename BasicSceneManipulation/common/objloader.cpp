#include "objloader.hpp"

#include <stdio.h>
#include <cstring>
#include <iostream>

#include <GL/glew.h>

#include "shader.hpp"

using namespace std;

Obj::Obj()
{
	m_texture = 0;
	m_textureUniformLocator = 0;
	m_matrixUniformLocator = 0;
	m_program = 0;
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

Obj* ObjFactory::loadOBJ( string filename )
{
	printf( "Loading OBJ file %s...\n", filename.c_str() );

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(filename.c_str(), "r");
	if( file == NULL )
	{
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}

	std::cout << "Loadeding file " << filename << std::endl;


	Mesh* mesh = NULL;
	Obj* outObj = new Obj();
	while( 1 )
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == char_traits<char>::eof())
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if ( strcmp( lineHeader, "o" ) == 0 )
		{
			if( mesh != NULL )
			{
				// For each vertex of each triangle
				for( unsigned int i=0; i<vertexIndices.size(); i++ )
				{
					// Get the indices of its attributes
					unsigned int vertexIndex = vertexIndices[i];
					unsigned int uvIndex = uvIndices[i];
					unsigned int normalIndex = normalIndices[i];

					// Get the attributes thanks to the index
					glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
					glm::vec2 uv = temp_uvs[ uvIndex-1 ];
					glm::vec3 normal = temp_normals[ normalIndex-1 ];

					// Put the attributes in buffers
					mesh->m_vertices.push_back(vertex);
					mesh->m_uvs.push_back(uv);
					mesh->m_normals.push_back(normal);

				}
			}
			// Add new mesh to the list
			mesh = new Mesh();
			outObj->m_meshList.push_back( mesh );
		}
		
		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			if( 9 == fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] ) )
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices    .push_back(uvIndex[0]);
				uvIndices    .push_back(uvIndex[1]);
				uvIndices    .push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			if( 6 == fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] ) )
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices    .push_back(uvIndex[0]);
				uvIndices    .push_back(uvIndex[1]);
				uvIndices    .push_back(uvIndex[2]);
			}
			if( 3 == fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] ) )
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
			}
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	if( mesh != NULL )
	{
		for( unsigned int i=0; i<vertexIndices.size(); i++ )
		{
			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
			glm::vec2 uv = temp_uvs[ uvIndex-1 ];
			glm::vec3 normal = temp_normals[ normalIndex-1 ];

			// Put the attributes in buffers
			mesh->m_vertices.push_back(vertex);
			mesh->m_uvs.push_back(uv);
			mesh->m_normals.push_back(normal);
		}
	}

	std::cout << "Loaded file with " << outObj->m_meshList.size() << " meshes" << std::endl;

	return outObj;
}

void Obj::setTexture( unsigned int textureFileRef )
{
	m_texture = textureFileRef;
}

void Obj::setShaders( string vertexShaderFilename, string fragmentShaderFilename )
{
	m_program = LoadShaders( vertexShaderFilename.c_str(), fragmentShaderFilename.c_str() );
	m_matrixUniformLocator = glGetUniformLocation(m_program, "MVP");
	m_textureUniformLocator  = glGetUniformLocation(m_program, "uSampler");
}

void Obj::setMVP( glm::mat4& MVP )
{
	m_mvp = MVP;
}

void Obj::draw()
{
	glUseProgram( m_program );

	glUniformMatrix4fv( m_matrixUniformLocator, 1, GL_FALSE, &m_mvp[0][0] );

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(m_textureUniformLocator, 0);

	for( int mesh=0, nMesh=m_meshList.size() ; mesh<nMesh ; ++mesh )
	{
		m_meshList[ mesh ]->draw();
	}

}

/******************************************************************************/
/****  					Mesh object									***********/
/******************************************************************************/

Mesh::Mesh()
{
	m_vertexBuffer = 0;
	m_uvBuffer = 0;
	m_normalBuffer = 0;
}

Mesh::~Mesh(){}

void Mesh::draw()
{
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
			3,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
	}

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glDisableVertexAttribArray(0);
	if( m_uvBuffer != 0 ) 	 glDisableVertexAttribArray(1);
	if( m_normalBuffer != 0 ) glDisableVertexAttribArray(2);
}




/******************************************************************************/
/**						Object Factory 									*******/
/******************************************************************************/

ObjFactory::ObjFactory(){}

ObjFactory::~ObjFactory(){}

static ObjFactory* instance = NULL;

ObjFactory* ObjFactory::getInstance()
{
	if( instance == NULL )
	{
		instance = new ObjFactory();
	}
	return instance;
}

void ObjFactory::deleteInstance()
{
	if( instance != NULL )
	{
		delete instance;
	}
}
