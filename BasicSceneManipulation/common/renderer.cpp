#include "renderer.hpp"

#include <iostream>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::renderScene( Scene* scene )
{
    m_perspectiveMatrix = scene->getCurrentCamera()->getProjectionMatrix();
    m_viewMatrix = scene->getCurrentCamera()->getViewMatrix();
    std::vector<Node*>& childrenList = scene->getChildren();
    glm::mat4 currentGlobalMatrix = scene->getLocalTranslation();
    for(std::vector<Node*>::iterator it=childrenList.begin() ; it!=childrenList.end() ; ++it)
    {
        renderNode((*it), currentGlobalMatrix);
    }
}

void Renderer::renderNode( Node* node, glm::mat4 currentGlobalMatrix )
{
    std::vector<Node*>& childrenList = node->getChildren();
    currentGlobalMatrix = currentGlobalMatrix * node->getLocalTranslation();
    for(std::vector<Node*>::iterator it=childrenList.begin() ; it!=childrenList.end() ; ++it)
    {
        (*it)->draw(currentGlobalMatrix, m_viewMatrix, m_perspectiveMatrix);
        renderNode((*it), currentGlobalMatrix);
    }
}
