#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "../gre/obj.hpp"

#include <vector>
#include <string>

#include <glm/glm.hpp>

using namespace std;

namespace gre
{

class Explosion: public Obj
{
public:
    Explosion();
    ~Explosion();

    void setAnimationStep( const float step=0.f, const float desplacementOffset=1.f );

    void setObject( Obj& object );

protected:
    glm::vec3 m_objectCenterPoint;

    std::vector<glm::vec3> m_zeroPositionVertices;
    std::vector<glm::vec2> m_zeroPositionUVs;
    std::vector<glm::vec3> m_zeroPositionNormals;

    std::vector<glm::vec3> m_triangleRotation;
};

}

#endif
