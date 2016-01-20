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
    void addPhader( Node* c );
    void removePhader( Node* c );
    Node* getChild( const unsigned int id );
    std::vector<Node*>& getChildren(){ return m_children; }
    // Main matrix getters
    glm::mat4 getGlobalMatrix();
    glm::mat4 getLocalMatrix();
    // Descomposed matrix getters
    glm::mat4 getGlobalTranslation(){ return m_globalTranslation; }
    glm::mat4 getLocalTranslation(){ return m_localTranslation; }
    glm::mat4 getGlobalRotation(){ return m_globalRotation; }
    glm::mat4 getLocalRotation(){ return m_localRotation; }
    glm::mat4 getGlobalScale(){ return m_globalScale; }
    glm::mat4 getLocalScale(){ return m_localScale; }
    // Global matrix setters
    void setGlobalMatrix( glm::mat4 matrix );
    void setLocalMatrix( glm::mat4 matrix );
    // Descompsed matrix setters
    void setGlobalTranslation( glm::mat4 translation );
    void setLocalTranslation( glm::mat4 translation );
    void setGlobalRotation( glm::mat4 rotation );
    void setLocalRotation( glm::mat4 rotation );
    void setGlobalScale( glm::mat4 scale );
    void setLocalScale( glm::mat4 scale );

    void update();
    void updateChildren();
    void updateAll();

    void setName( const std::string name ){ m_name = name; }
    std::string& getName(){ return m_name; }

    void getViewportSize( int& width, int & height );

private:
    std::vector<Node*> m_children;
    Node* m_father;

protected:
    glm::mat4 m_localMatrix;
    glm::mat4 m_globalMatrix;

    glm::mat4 m_localTranslation;
    glm::mat4 m_localRotation;
    glm::mat4 m_localScale;
    glm::mat4 m_globalTranslation;
    glm::mat4 m_globalRotation;
    glm::mat4 m_globalScale;

    glm::vec3 m_scale;
    float m_rotationAngle;
	glm::vec3 m_rotationAxis;
    glm::vec3 m_translation;

    std::string m_name;
};

}

#endif
