#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

using namespace std;

class Node
{
public:
    Node();
    ~Node();

    virtual void draw();
    void addChild( Node* c );
    Node* getChild( const unsigned int id );

    void setPhader( Node* p );

    glm::mat4 getGlobalTranslation(){ return m_globalTranslation; }
    glm::mat4 getLocalTranslation(){ return m_localTranslation; }

    void setGlobalTranslation( glm::mat4 translation );
    void setLocalTranslation( glm::mat4 translation );

    void updateGlobalTranslation(){m_globalTranslation = m_phader->getGlobalTranslation() * m_localTranslation;}

    void updateChildrenTranslation();

private:
    std::vector<Node*> m_children;

    Node* m_phader;

protected:
    glm::mat4 m_localTranslation;
    glm::mat4 m_globalTranslation;
};

#endif
