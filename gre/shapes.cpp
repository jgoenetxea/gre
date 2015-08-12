/*
 * Shapes.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: goe
 */

#include "shapes.hpp"

using namespace gre;

Shapes::Shapes()
{
}

Shapes::~Shapes()
{
}

Obj* Shapes::getCube()
{
    Obj* obj = NULL;

    return obj;
}

Obj* Shapes::getQuad()
{
    Obj* obj = new Obj();
    Mesh* mesh = new Mesh();

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uv;
    std::vector<glm::vec3> temp_normals;

    temp_vertices.push_back(glm::vec3(-1.f, -1.f, 0.f));
    temp_vertices.push_back(glm::vec3(1.f, -1.f, 0.f));
    temp_vertices.push_back(glm::vec3(1.f, 1.f, 0.f));
    temp_vertices.push_back(glm::vec3(-1.f, 1.f, 0.f));

    temp_uv.push_back(glm::vec2( 0, 0 ));
    temp_uv.push_back(glm::vec2( 1, 0 ));
    temp_uv.push_back(glm::vec2( 1, -1 ));
    temp_uv.push_back(glm::vec2( 0, -1 ));

    temp_normals.push_back(glm::vec3(0.f, 0.f, -1.f));
    temp_normals.push_back(glm::vec3(0.f, 0.f, -1.f));
    temp_normals.push_back(glm::vec3(0.f, 0.f, -1.f));
    temp_normals.push_back(glm::vec3(0.f, 0.f, -1.f));

    std::vector<unsigned short> temp_indices;

    temp_indices.push_back(0);
    temp_indices.push_back(1);
    temp_indices.push_back(2);
    temp_indices.push_back(0);
    temp_indices.push_back(2);
    temp_indices.push_back(3);

    mesh->m_vertexIndex = temp_indices;

    obj->m_vertices = temp_vertices;
    obj->m_uvs = temp_uv;
    obj->m_normals = temp_normals;
    obj->m_meshList.push_back(mesh);

    return obj;
}
