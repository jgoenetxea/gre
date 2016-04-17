#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "node.hpp"
#include "scene.hpp"

using namespace std;

namespace gre
{

class Renderer
{
    enum RenderType{
        ActiveRender, // Use the local data of the nodes and compute the location and the orientation on rendering.
        PassiveRender // Use the stored information in the nodes and use only the main global matrix for each node.
    };
public:
    ~Renderer();

    static Renderer* getInstance()
    {
        static Renderer* _instance = new Renderer();
        return _instance;
    }

    void renderScene( Scene* scene, RenderType type=ActiveRender );

private:
    Renderer();

    glm::mat4 m_perspectiveMatrix;
    glm::mat4 m_viewMatrix;

    void renderNode( Node* node, glm::mat4 currentGlobalMatrix );
    void renderNodeActive( Node* node, glm::mat4 previousGlobalMatrix );
};

}

#endif
