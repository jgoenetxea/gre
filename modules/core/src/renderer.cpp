#include "renderer.hpp"

#include <iostream>
#include "logger.h"
#define LOG_TAG "Renderer"

using namespace gre;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::renderScene( Scene* scene, RenderType type )
{
    m_perspectiveMatrix = scene->getCurrentCamera()->getProjectionMatrix();
    m_viewMatrix = scene->getCurrentCamera()->getViewMatrix();
    glm::mat4 currentGlobalMatrix = scene->getGlobalMatrix();

    if(type == ActiveRender)
    {
        renderNodeActive(scene, currentGlobalMatrix);
    }
    else if(type == PassiveRender)
    {
        renderNode(scene, currentGlobalMatrix);
    }
}

void Renderer::renderNode( Node* node, glm::mat4 currentGlobalMatrix )
{
    std::vector<Node*>& childrenList = node->getChildren();
    node->draw(currentGlobalMatrix, m_viewMatrix, m_perspectiveMatrix);
    for(std::vector<Node*>::iterator it=childrenList.begin() ; it!=childrenList.end() ; ++it)
    {
        //LOGD("Rendering: %s", n->getName().c_str());
        std::cout << (*it)->getName() << "::G=" << (*it)->getGlobalMatrix()[3][0] << "::L=" << (*it)->getLocalMatrix()[3][0] << std::endl;
        renderNode((*it), (*it)->getGlobalMatrix());
    }
}

void Renderer::renderNodeActive( Node* node, glm::mat4 previousGlobalMatrix )
{
    // Get node data
    glm::mat4 localRotation = node->getLocalRotation();
    glm::mat4 localTranslation = node->getLocalTranslation();
    glm::mat4 localScale = node->getLocalScale();
    // Compute inverse matrix for previous global matrix
    glm::mat4 previousGlobalMatrixInverse = glm::inverse(previousGlobalMatrix);
    // Compute current matrix
    glm::mat4 currentGlobalMatrix = previousGlobalMatrix * localScale * localTranslation * localRotation * previousGlobalMatrixInverse;
    // Draw
    node->draw(currentGlobalMatrix, m_viewMatrix, m_perspectiveMatrix);
    // Call children
    std::vector<Node*>& childrenList = node->getChildren();
    for(std::vector<Node*>::iterator it=childrenList.begin() ; it!=childrenList.end() ; ++it)
    {
        //LOGD("Rendering: %s", n->getName().c_str());
        //std::cout << (*it)->getName() << "::G=" << (*it)->getGlobalMatrix()[3][0] << "::L=" << (*it)->getLocalMatrix()[3][0] << std::endl;
        renderNodeActive((*it), currentGlobalMatrix);
    }
}

