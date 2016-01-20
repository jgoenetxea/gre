#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <vector>
#include <string>

#include "node.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace std;

namespace gre
{

class Transformation : public Node
{
public:
	Transformation();
    ~Transformation();

    void setScale(glm::vec3 scale);
    void setRotation(float angle, glm::vec3 axis);
    void setTranslation(glm::vec3 translation);

    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 scale);

private:
    //void computeLocalMatrix();
};

}

#endif
