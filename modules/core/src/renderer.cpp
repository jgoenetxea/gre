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

void Renderer::renderScene( Scene* scene )
{
    m_perspectiveMatrix = scene->getCurrentCamera()->getProjectionMatrix();
    m_viewMatrix = scene->getCurrentCamera()->getViewMatrix();
    glm::mat4 currentGlobalMatrix = scene->getLocalTranslation();
    renderNode(scene, currentGlobalMatrix);
}

void Renderer::renderNode( Node* node, glm::mat4 currentGlobalMatrix )
{
    std::vector<Node*>& childrenList = node->getChildren();
    currentGlobalMatrix = currentGlobalMatrix * node->getLocalTranslation();
    //currentGlobalMatrix = node->getLocalTranslation() * currentGlobalMatrix;
    node->draw(currentGlobalMatrix, m_viewMatrix, m_perspectiveMatrix);
    for(std::vector<Node*>::iterator it=childrenList.begin() ; it!=childrenList.end() ; ++it)
    {
        //LOGD("Rendering: %s", n->getName().c_str());
        renderNode((*it), currentGlobalMatrix);
    }
}
