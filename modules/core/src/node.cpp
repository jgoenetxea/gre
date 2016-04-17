#include "node.hpp"

#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>
//#include<GLES2/gl2.h>

#include "logger.h"
#define LOG_TAG "Node"

using namespace gre;

Node::Node(): m_father(NULL)
{
    m_globalMatrix = glm::mat4(1.0f);
    m_localMatrix= glm::mat4(1.0f);
    m_scale = glm::vec3(1.0f);
    m_rotationAngle = 0.0f;
	m_rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    m_translation = glm::vec3(0.0f);

    m_localTranslation = glm::mat4(1.0f);
    m_localRotation = glm::mat4(1.0f);
    m_localScale = glm::mat4(1.0f);
    m_globalTranslation = glm::mat4(1.0f);
    m_globalRotation = glm::mat4(1.0f);
    m_globalScale = glm::mat4(1.0f);

    m_name = "Unamed";
}

Node::~Node()
{
    for(std::vector<Node*>::iterator it=m_children.begin() ; it!=m_children.end() ; ++it)
    {
        if(*it != NULL)
        {
            delete (*it);
            *it = NULL;
        }
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
    c->addPhader(this);
}

void Node::removeChild( Node* c )
{
	for (std::vector<Node*>::iterator it = m_children.begin(); it != m_children.end(); )
	{
		if( (*it) == c )
		{
			//LOGD("Removing: %p", c);
            //delete * it;
			it = m_children.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Node* Node::getChild( const unsigned int id )
{
    if( id < static_cast<unsigned int>( m_children.size() ) )
    {
        return m_children[id];
    }
    return NULL;
}

void Node::addPhader( Node* c )
{
    m_father = c;
}

glm::mat4 Node::getGlobalMatrix()
{
    m_globalMatrix = m_globalTranslation * m_globalRotation * m_globalScale;
    return m_globalMatrix;
}

glm::mat4 Node::getLocalMatrix()
{
    m_localMatrix = m_localTranslation * m_localRotation * m_localScale;
    return m_localMatrix;
}

void Node::setGlobalMatrix( glm::mat4 transformation )
{
    assert(false);
    // Define the global translation
    m_globalMatrix = transformation;
    // Compute the local translation for the defined global
    if( m_father != NULL )
    {
        m_localMatrix = m_globalMatrix * glm::inverse(m_father->getGlobalMatrix());
    }
}

void Node::setLocalMatrix( glm::mat4 transformation )
{
    assert(false);
    m_localMatrix = transformation;
    if( m_father != NULL )
    {
        m_globalMatrix = m_localMatrix * m_father->getGlobalMatrix();
    }
}

void Node::setGlobalTranslation( glm::mat4 translation )
{
    // Define the global translation
    m_globalTranslation = translation;
    // Compute the local translation for the defined global
    if( m_father != NULL )
    {
        m_localTranslation = m_globalTranslation * glm::inverse(m_father->getGlobalTranslation());
    }
    updateChildren();
}

void Node::setLocalTranslation( glm::mat4 translation )
{
    m_localTranslation = translation;
    if( m_father != NULL )
    {
        m_globalTranslation = m_localTranslation * m_father->getGlobalTranslation();
    }
    updateChildren();
}

void Node::setGlobalRotation( glm::mat4 rotation )
{
    // Define the global translation
    m_globalRotation = rotation;
    // Compute the local translation for the defined global
    if( m_father != NULL )
    {
        m_localRotation = m_globalRotation * glm::inverse(m_father->getGlobalRotation());
    }
    updateChildren();
}

void Node::setLocalRotation( glm::mat4 rotation )
{
    m_localRotation = rotation;
    if( m_father != NULL )
    {
        m_globalRotation = m_localRotation * m_father->getGlobalRotation();
    }
    updateChildren();
}

void Node::setGlobalScale( glm::mat4 scale )
{
    // Define the global translation
    m_globalScale = scale;
    // Compute the local translation for the defined global
    if( m_father != NULL )
    {
        m_localScale = m_globalScale * glm::inverse(m_father->getGlobalScale());
    }
    updateChildren();
}

void Node::setLocalScale( glm::mat4 scale )
{
    m_localScale = scale;
    if( m_father != NULL )
    {
        m_globalScale = m_localScale * m_father->getGlobalScale();
    }
    updateChildren();
}

void Node::getViewportSize( int& width, int & height )
{
    GLint viewport[4];

    glGetIntegerv( GL_VIEWPORT, viewport );

    width = viewport[2] - viewport[0];
    height = viewport[3] - viewport[1];
}

void Node::update()
{
    if(m_father != NULL)
    {
        m_globalTranslation = m_localTranslation * m_father->getGlobalTranslation();
        m_globalRotation = m_localRotation * m_father->getGlobalRotation();
        m_globalScale = m_localScale * m_father->getGlobalScale();
    }
}

void Node::updateAll()
{
    update();
    updateChildren();
}

void Node::updateChildren()
{
    for(std::vector<Node*>::iterator it=m_children.begin() ; it!=m_children.end() ; ++it)
    {
        (*it)->updateAll();
    }
}
