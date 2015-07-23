#include "node.hpp"

#include <iostream>

Node::Node()
{
    m_globalTranslation = glm::mat4(1);
    m_localTranslation = glm::mat4(1);
    m_phader = NULL;
}

Node::~Node()
{
    for(std::vector<Node*>::iterator it=m_children.begin() ; it!=m_children.end() ; ++it)
    {
        delete (*it);
    }
}

void Node::draw()
{
    for(std::vector<Node*>::iterator it=m_children.begin() ; it!=m_children.end() ; ++it)
    {
        (*it)->draw();
    }
}

void Node::addChild( Node* c )
{
    c->setPhader(this);
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

void Node::setPhader( Node* p )
{
    m_phader = p;

    updateGlobalTranslation();
}

void Node::setGlobalTranslation( glm::mat4 translation )
{
    // Define the global translation
    m_globalTranslation = translation;
    // Compute the local translation for the defined global
    m_localTranslation = m_phader->getGlobalTranslation() * glm::inverse(m_globalTranslation);


}

void Node::setLocalTranslation( glm::mat4 translation )
{
    m_localTranslation = translation;
    m_globalTranslation = m_localTranslation * m_phader->getGlobalTranslation();

    for(std::vector<Node*>::iterator it=m_children.begin() ; it!=m_children.end() ; ++it)
    {
        (*it)->updateChildrenTranslation();
    }
}

void Node::updateChildrenTranslation()
{
    m_globalTranslation = m_localTranslation * m_phader->getGlobalTranslation();
}
