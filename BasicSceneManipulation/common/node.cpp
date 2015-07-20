#include "node.hpp"

#include <iostream>

Node::Node()
{

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
