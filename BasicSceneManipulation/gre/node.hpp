#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

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
    Node* getChild( const unsigned int id );
    std::vector<Node*>& getChildren(){ return m_children; }

    glm::mat4 getGlobalTranslation(){ return m_globalTranslation; }
    glm::mat4 getLocalTranslation(){ return m_localTranslation; }

    void setGlobalTranslation( glm::mat4 translation );
    void setLocalTranslation( glm::mat4 translation );

    void setName( const std::string name ){ m_name = name; }
    std::string& getName(){ return m_name; }

private:
    std::vector<Node*> m_children;

protected:
    glm::mat4 m_localTranslation;
    glm::mat4 m_globalTranslation;

    std::string m_name;
};

}

#endif
