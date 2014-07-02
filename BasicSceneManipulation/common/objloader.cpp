#include "objloader.hpp"

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "shaderProgram.hpp"

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

int ObjFactory::isEqualStored( 	std::vector<unsigned short>& vertexIndices,
									std::vector<unsigned short>& uvIndices,
									std::vector<unsigned short>& normalIndices,
									unsigned int currentIndex
									)
{
	unsigned int n_uIndex = uvIndices.size();
	unsigned int n_nIndex = normalIndices.size();
	unsigned int n_vIndex = vertexIndices.size();
	int index = -1;

	// In the case where only the vertex values are indexed
	if( n_vIndex >0 && n_nIndex == 0 && n_uIndex == 0 )
	{
		unsigned short vertexIndexToBeFound = vertexIndices[currentIndex];
		bool valueFound = false;
		for( unsigned int i=0 ; i<currentIndex && !valueFound ; ++i )
		{
			if( vertexIndices[i] == vertexIndexToBeFound )
			{
				valueFound = true;
				index = i;
			}
		}
	}
	// In the case where only vertex and normal values are indexed
	else if( n_vIndex >0 && n_nIndex > 0 && n_uIndex == 0 )
	{
		unsigned short vertexIndexToBeFound = vertexIndices[currentIndex];
		unsigned short normalIndexToBeFound = normalIndices[currentIndex];
		bool valueFound = false;
		for( unsigned int i=0 ; i<currentIndex && !valueFound ; ++i )
		{
			if( vertexIndices[i] == vertexIndexToBeFound )
			{
				// OK, the vertices has the same index value, but...the normal has?
				if( normalIndices[i] == normalIndexToBeFound )
				{
					valueFound = true;
					index = i;
				}
			}
		}
	}

	// In the case where only vertex and uv values are indexed
	else if( n_vIndex >0 && n_nIndex == 0 && n_uIndex > 0 )
	{
		unsigned short vertexIndexToBeFound = vertexIndices[currentIndex];
		unsigned short uvIndexToBeFound = uvIndices[currentIndex];
		bool valueFound = false;
		for( unsigned int i=0 ; i<currentIndex && !valueFound ; ++i )
		{
			if( vertexIndices[i] == vertexIndexToBeFound )
			{
				// OK, the vertices has the same index value, but...the uv has?
				if( uvIndices[i] == uvIndexToBeFound )
				{
					valueFound = true;
					index = i;
				}
			}
		}
	}

	// In the case where all the values are indexed
	else if( n_vIndex >0 && n_nIndex > 0 && n_uIndex > 0 )
	{
		unsigned short vertexIndexToBeFound = vertexIndices[currentIndex];
		unsigned short normalIndexToBeFound = normalIndices[currentIndex];
		unsigned short uvIndexToBeFound = uvIndices[currentIndex];

		//cout << currentIndex << " => " << vertexIndexToBeFound+1 << " " << uvIndexToBeFound+1 << " " << normalIndexToBeFound+1;

		bool valueFound = false;
		for( unsigned int i=0 ; i<currentIndex && !valueFound ; ++i )
		{
			if( vertexIndices[i] == vertexIndexToBeFound )
			{
				//int normal = normalIndices[i];	// DEBUG
				//int uv = uvIndices[i];			// DEBUG
				// OK, the vertices has the same index value, but...the normal has?
				if( normalIndices[i] == normalIndexToBeFound )
				{
					// ... and what about the uv values?
					if( uvIndices[i] == uvIndexToBeFound )
					{
						valueFound = true;
						index = i;
					}
				}
			}
		}

		if( valueFound )
			cout << "\tFound: " << index << endl;
		else
			cout << "\tNot Found" << endl;
	}

	return index; // If none of the previous condition is reached, return false
}

Obj* ObjFactory::loadOBJ( string filename )
{
	printf( "Loading OBJ file %s...\n", filename.c_str() );

	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(filename.c_str(), "r");
	if( file == NULL )
	{
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        return NULL;
	}

	std::cout << "Loadeding file " << filename << std::endl;

	char name[255];
	Mesh* mesh = NULL;
	Obj* outObj = new Obj();
	int previousVertexListSize = 0;
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
				// Before store the data, is necesary to align the vertex/uv/normal information.
				// The info is stored in the obj object, and the indices are stored in the mesh instance
				unsigned int n_vIndex = vertexIndices.size();
				unsigned int n_uIndex = uvIndices.size();
				unsigned int n_nIndex = normalIndices.size();
				int previousIndexValue = -1;
				for( unsigned int fIndex=0 ; fIndex<n_vIndex ; ++fIndex )
				{
					// See if we have to add a new vertex data or the vertex is just in the vertex list
					previousIndexValue = isEqualStored( vertexIndices, uvIndices, normalIndices, fIndex );
					if( previousIndexValue >= 0 )
					{
						// add the vertex index. The uv and normal values are in the same position, so only the vertex index is needed
						mesh->m_vertexIndex.push_back( previousVertexListSize + previousIndexValue );
					}
					else
					{
						// The current vertex array size is the index of the next item
						int currentIndexArrayIndex = outObj->m_vertices.size();
						// Add the next item, and store the index in the index array
						int i = vertexIndices[fIndex];
						outObj->m_vertices.push_back( temp_vertices[i] );
						mesh->m_vertexIndex.push_back( currentIndexArrayIndex );
						// The index is equal for the rest of the arrays, so, if would necessary, story their values in each array
						if( n_uIndex > 0 ) outObj->m_uvs.push_back( temp_uvs[uvIndices[fIndex]] );
						if( n_nIndex > 0 ) outObj->m_normals.push_back( temp_normals[normalIndices[fIndex]] );

					}
				}
			}
			// Add new mesh to the list
			mesh = new Mesh();
			//fscanf(file, "%s\n", name );
			//string name2 = name;
			//mesh->setName( name2 );
			outObj->m_meshList.push_back( mesh );
			previousVertexListSize = outObj->m_vertices.size();
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

			// The lists in the obj file are indexed from 1 to n, but in c++, the lists are indexed from 0 to n-1
			// To fix this, the indices are reduced in one (-1) on reading

			if( 9 == fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] ) )
			{
				vertexIndices.push_back( vertexIndex[0]-1 );
				vertexIndices.push_back( vertexIndex[1]-1 );
				vertexIndices.push_back( vertexIndex[2]-1 );
				uvIndices    .push_back( uvIndex[0]-1 );
				uvIndices    .push_back( uvIndex[1]-1 );
				uvIndices    .push_back( uvIndex[2]-1 );
				normalIndices.push_back( normalIndex[0]-1 );
				normalIndices.push_back( normalIndex[1]-1 );
				normalIndices.push_back( normalIndex[2]-1 );
			}
			if( 6 == fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] ) )
			{
				vertexIndices.push_back( vertexIndex[0]-1 );
				vertexIndices.push_back( vertexIndex[1]-1 );
				vertexIndices.push_back( vertexIndex[2]-1 );
				uvIndices    .push_back( uvIndex[0]-1 );
				uvIndices    .push_back( uvIndex[1]-1 );
				uvIndices    .push_back( uvIndex[2]-1 );
			}
			if( 6 == fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] ) )
			{
				vertexIndices.push_back( vertexIndex[0]-1 );
				vertexIndices.push_back( vertexIndex[1]-1 );
				vertexIndices.push_back( vertexIndex[2]-1 );
				normalIndices.push_back( normalIndex[0]-1 );
				normalIndices.push_back( normalIndex[1]-1 );
				normalIndices.push_back( normalIndex[2]-1 );
			}
			if( 3 == fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] ) )
			{
				vertexIndices.push_back( vertexIndex[0]-1 );
				vertexIndices.push_back( vertexIndex[1]-1 );
				vertexIndices.push_back( vertexIndex[2]-1 );
			}
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// Same as before, for the last readed mesh
	if( mesh != NULL )
	{
		cout << "Ridden vertices:" << endl;
		for(int index=0, n=temp_vertices.size() ; index<n ; ++index)
		{
			cout << temp_vertices[index].x << " " << temp_vertices[index].y << " " << temp_vertices[index].z << endl;
		}
		cout << endl;

		// Before store the data, is necesary to align the vertex/uv/normal information.
		// The info is stored in the obj object, and the indices are stored in the mesh instance
		unsigned int n_vIndex = vertexIndices.size();
		unsigned int n_uIndex = uvIndices.size();
		unsigned int n_nIndex = normalIndices.size();
		int previousIndexValue = -1;
		for( unsigned int fIndex=0 ; fIndex<n_vIndex ; ++fIndex )
		{
			// See if we have to add a new vertex data or the vertex is just in the vertex list
			previousIndexValue = isEqualStored( vertexIndices, uvIndices, normalIndices, fIndex );
			if( previousIndexValue >= 0 )
			{
				// Find the index stored in the defined index position
				int currentIndex = mesh->m_vertexIndex[ previousVertexListSize + previousIndexValue ];
				// add the vertex index. The uv and normal values are in the same position, so only the vertex index is needed
				mesh->m_vertexIndex.push_back( currentIndex );
			}
			else
			{
				// The current vertex array size is the index of the next item
				int currentIndexArrayIndex = outObj->m_vertices.size();
				// Add the next item, and store the index in the index array
				int i = vertexIndices[fIndex];
				//cout << temp_vertices[i].x << " " << temp_vertices[i].y << " " << temp_vertices[i].z << endl;
				outObj->m_vertices.push_back( temp_vertices[i] );
				mesh->m_vertexIndex.push_back( currentIndexArrayIndex );
				// The index is equal for the rest of the arrays, so, if would necessary, story their values in each array
				if( n_uIndex > 0 ) outObj->m_uvs.push_back( temp_uvs[uvIndices[fIndex]] );
				if( n_nIndex > 0 ) outObj->m_normals.push_back( temp_normals[normalIndices[fIndex]] );

			}
		}
	}

//	std::cout << "Loaded file with " << outObj->m_meshList.size() << " meshes" << std::endl;
//	std::cout << "\tvertices: " << outObj->m_vertices.size() << std::endl;
//	std::cout << "\tuvs     : " << outObj->m_uvs.size() << std::endl;
//	std::cout << "\tnormals : " << outObj->m_normals.size() << std::endl;
//
//
//	{
//		ofstream ofile("loadedData.txt");
//		ofile << "Object has " << outObj->m_vertices.size() << " vertices, " << outObj->m_uvs.size() << " uvs and " << outObj->m_normals.size() << " normals" << endl;
//		for( int iMesh=0,nMesh=outObj->m_meshList.size() ; iMesh<nMesh ; ++iMesh )
//		{
//			ofile << "Mesh #" << iMesh+1 << endl;
//			ofile << "\tIndices: " << outObj->m_meshList[iMesh]->m_vertexIndex.size() << endl;
//			ofile << "Indexed vertices:" << endl;
//			int ci=0;
//			for( int i=0,n=outObj->m_meshList[iMesh]->m_vertexIndex.size() ; i<n ; ++i )
//			{
//				ci = outObj->m_meshList[iMesh]->m_vertexIndex[i];
//				ofile << "\t" << outObj->m_vertices[ci].x << " " << outObj->m_vertices[ci].y << " " << outObj->m_vertices[ci].z << endl;
//			}
//		}
//		ofile << "//************************************************************************//" << endl;
//		ofile << "Vertices:" << endl;
//		for( int ivert=0,nvert=outObj->m_vertices.size() ; ivert<nvert ; ++ivert )
//		{
//			ofile << outObj->m_vertices[ivert].x << " " << outObj->m_vertices[ivert].y << " " << outObj->m_vertices[ivert].z << endl;
//		}
//		ofile << "Indices:" << endl;
//		for( int iMesh=0,nMesh=outObj->m_meshList.size() ; iMesh<nMesh ; ++iMesh )
//		{
//			ofile << "Mesh #" << iMesh+1 << endl;
//			for( int i=0,nIndex=outObj->m_meshList[iMesh]->m_vertexIndex.size() ; i<nIndex ; ++i )
//			{
//				ofile << outObj->m_meshList[iMesh]->m_vertexIndex[i] << endl;
//			}
//		}
//		ofile.close();
//	}

	return outObj;
}

void Obj::setTexture( unsigned int textureFileRef )
{
	m_texture = textureFileRef;
}

void Obj::setShaders( string vertexShaderFilename, string fragmentShaderFilename )
{
    m_program = ProgramGenerator::makeProgramUsingFiles( vertexShaderFilename, fragmentShaderFilename );
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
