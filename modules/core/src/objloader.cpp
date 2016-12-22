#include "objloader.hpp"

#include <stdio.h>
#include <string.h>

// #define EXPORT_LOADED_DATA

#if defined(EXPORT_LOADED_DATA)
#include <fstream>
#endif

using namespace std;
using namespace gre;

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
    cout << "Loading OBJ file " << filename.c_str() << "...\n";

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

#if defined(EXPORT_LOADED_DATA)
    std::cout << "Loaded file with " << outObj->m_meshList.size() << " meshes" << std::endl;
    std::cout << "\tvertices: " << outObj->m_vertices.size() << std::endl;
    std::cout << "\tuvs     : " << outObj->m_uvs.size() << std::endl;
    std::cout << "\tnormals : " << outObj->m_normals.size() << std::endl;


    {
        ofstream ofile;
        ofile.open("loadedData.txt");
        ofile << "Object has " << outObj->m_vertices.size() << " vertices, " << outObj->m_uvs.size() << " uvs and " << outObj->m_normals.size() << " normals" << endl;
        for( int iMesh=0,nMesh=outObj->m_meshList.size() ; iMesh<nMesh ; ++iMesh )
        {
            ofile << "Mesh #" << iMesh+1 << endl;
            ofile << "\tIndices: " << outObj->m_meshList[iMesh]->m_vertexIndex.size() << endl;
            ofile << "Indexed vertices:" << endl;
            int ci=0;
            for( int i=0,n=outObj->m_meshList[iMesh]->m_vertexIndex.size() ; i<n ; ++i )
            {
                ci = outObj->m_meshList[iMesh]->m_vertexIndex[i];
                ofile << "\t" << outObj->m_vertices[ci].x << " " << outObj->m_vertices[ci].y << " " << outObj->m_vertices[ci].z << endl;
            }
        }
        ofile << "//************************************************************************//" << endl;
        ofile << "Vertices:" << endl;
        for( int ivert=0,nvert=outObj->m_vertices.size() ; ivert<nvert ; ++ivert )
        {
            ofile << outObj->m_vertices[ivert].x << " " << outObj->m_vertices[ivert].y << " " << outObj->m_vertices[ivert].z << endl;
        }
        ofile << "UVs:" << endl;
        for( int iuv=0,nuv=outObj->m_uvs.size() ; iuv<nuv ; ++iuv )
        {
            ofile << outObj->m_uvs[iuv].x << " " << outObj->m_uvs[iuv].y << endl;
        }
        ofile << "Normals:" << endl;
        for( int inorm=0,nnorm=outObj->m_normals.size() ; inorm<nnorm ; ++inorm )
        {
            ofile << outObj->m_normals[inorm].x << " " << outObj->m_normals[inorm].y << " " << outObj->m_normals[inorm].z << endl;
        }
        ofile << "Indices:" << endl;
        for( int iMesh=0,nMesh=outObj->m_meshList.size() ; iMesh<nMesh ; ++iMesh )
        {
            ofile << "Mesh #" << iMesh+1 << endl;
            for( int i=0,nIndex=outObj->m_meshList[iMesh]->m_vertexIndex.size() ; i<nIndex ; ++i )
            {
                ofile << outObj->m_meshList[iMesh]->m_vertexIndex[i] << endl;
            }
        }
        ofile.close();
    }
#endif

	return outObj;
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
