#include "explosion.hpp"

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include <stdlib.h>     /* srand, rand */

using namespace std;
using namespace gre;

Explosion::Explosion()
{
    m_objectCenterPoint = glm::vec3(0,0,0);
}

Explosion::~Explosion()
{

}

void Explosion::setAnimationStep( const float step, const float maxOffset )
{
//    float currentStep = maxOffset * step;
//    for( int m=0 ; m<meshList.size() ; ++m)
//    {
//        for( int i=0 ; i<meshList.size() ; ++i)
//        {

//        }
//        int a = meshList->m_vertexIndex[i];
//        int b = meshList->m_vertexIndex[i+1];
//        int c = meshList->m_vertexIndex[i+2];
//    }
}

void Explosion::setObject( Obj& object )
{
    std::vector<glm::vec3> originalVertices;
    std::vector<glm::vec2> originalUVs;
    std::vector<glm::vec3> originalNormals;

    m_zeroPositionVertices.clear();
    m_zeroPositionUVs.clear();
    m_zeroPositionNormals.clear();
    m_triangleRotation.clear();

    object.getInnerData( originalVertices, originalUVs, originalNormals );
    // get mesh data and triangle indices
    std::vector<Mesh*>& meshList = object.getMeshList();
    // Generate a mesh for each triangle
    for( std::vector<Mesh*>::iterator mesh = meshList.begin() ; mesh != meshList.end() ; ++mesh )
    {
        for( int i = 0 ; i < static_cast<int>((*mesh)->m_vertexIndex.size()) ; i=i+3 )
        {
            int a = (*mesh)->m_vertexIndex[i];
            int b = (*mesh)->m_vertexIndex[i+1];
            int c = (*mesh)->m_vertexIndex[i+2];
            // Generate vertex list
            m_zeroPositionVertices.push_back( originalVertices[a] );
            m_zeroPositionVertices.push_back( originalVertices[b] );
            m_zeroPositionVertices.push_back( originalVertices[c] );
            m_zeroPositionUVs.push_back( originalUVs[a] );
            m_zeroPositionUVs.push_back( originalUVs[b] );
            m_zeroPositionUVs.push_back( originalUVs[c] );
            m_zeroPositionNormals.push_back( originalNormals[a] );
            m_zeroPositionNormals.push_back( originalNormals[b] );
            m_zeroPositionNormals.push_back( originalNormals[c] );
            // Generate mesh
            Mesh* m = new Mesh();
            m->m_vertexIndex.push_back(i);
            m->m_vertexIndex.push_back(i+1);
            m->m_vertexIndex.push_back(i+2);
            m_meshList.push_back(m);
            // generate random rotation scheme
            glm::vec3 rot(rand(), rand(), rand());
            m_triangleRotation.push_back(glm::normalize(rot));
        }
    }
    // Store the meshes
    setInnerData( m_zeroPositionVertices, m_zeroPositionUVs, m_zeroPositionNormals );

    m_objectCenterPoint = glm::vec3(0,0,0);
    // find the center point of the object using the middle point of the elements
    for( std::vector<glm::vec3>::iterator it = m_vertices.begin() ; it != m_vertices.end() ; ++it )
    {
        m_objectCenterPoint += (*it);
    }
    int nVertices = static_cast<int>(m_vertices.size());
    m_objectCenterPoint.x /= nVertices;
    m_objectCenterPoint.y /= nVertices;
    m_objectCenterPoint.z /= nVertices;
}
