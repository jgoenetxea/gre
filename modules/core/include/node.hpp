#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;

namespace gre
{

class Node
{
public:
    Node();
    ~Node();

    virtual void draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective );
    void addChild( Node* c );
    void removeChild( Node* c );
    Node* getChild( const unsigned int id );
    std::vector<Node*>& getChildren(){ return m_children; }

    glm::mat4 getGlobalTranslation(){ return m_globalTransformation; }
    glm::mat4 getLocalTranslation(){ return m_localTransformation; }

    void setGlobalTranslation( glm::mat4 transformation );
    void setLocalTranslation( glm::mat4 transformation );

    void setName( const std::string name ){ m_name = name; }
    std::string& getName(){ return m_name; }

    void getViewportSize( int& width, int & height );

private:
    std::vector<Node*> m_children;

protected:
    glm::mat4 m_localTransformation;
    glm::mat4 m_globalTransformation;

    glm::vec3 m_scale;
    float m_rotationAngle;
	glm::vec3 m_rotationAxis;
    glm::vec3 m_translation;

    std::string m_name;
};

}

#endif
