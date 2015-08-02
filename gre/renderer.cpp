#include "renderer.hpp"

#include <iostream>

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
    Node* n;
    for(std::vector<Node*>::iterator it=childrenList.begin() ; it!=childrenList.end() ; ++it)
    {
        n = (*it);
        std::cout << "Rendering: " << n->getName() << std::endl;
        n->draw(currentGlobalMatrix, m_viewMatrix, m_perspectiveMatrix);
        renderNode((*it), currentGlobalMatrix);
    }
}
