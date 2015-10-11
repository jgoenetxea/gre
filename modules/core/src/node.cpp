#include "node.hpp"

#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>
//#include<GLES2/gl2.h>

using namespace gre;

Node::Node()
{
    m_globalTranslation = glm::mat4(1);
    m_localTranslation = glm::mat4(1);
    m_name = "Unamed";
}

Node::~Node()
{
    for(std::vector<Node*>::iterator it=m_children.begin() ; it!=m_children.end() ; ++it)
    {
        delete (*it);
    }
}

void Node::draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective )
{
    /*for(std::vector<Node*>::iterator it=m_children.begin() ; it!=m_children.end() ; ++it)
    {
        (*it)->draw();
    }*/
}

void Node::addChild( Node* c )
{
    m_children.push_back(c);
}

Node* Node::getChild( const unsigned int id )
{
    if( id < static_cast<unsigned int>( m_children.size() ) )
    {
        return m_children[id];
    }
    return NULL;
}

void Node::setGlobalTranslation( glm::mat4 translation )
{
    return;
    // Define the global translation
   // m_globalTranslation = translation;
    // Compute the local translation for the defined global
    //m_localTranslation = m_phader->getGlobalTranslation() * glm::inverse(m_globalTranslation);


}

void Node::setLocalTranslation( glm::mat4 translation )
{
    m_localTranslation = translation;
    //m_globalTranslation = m_localTranslation * m_phader->getGlobalTranslation();
}

void Node::getViewportSize( int& width, int & height )
{
    GLint viewport[4];

    glGetIntegerv( GL_VIEWPORT, viewport );

    width = viewport[2] - viewport[0];
    height = viewport[3] - viewport[1];
}
