#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "node.hpp"
#include "scene.hpp"

using namespace std;

class Renderer
{
public:
    ~Renderer();

    static Renderer* getInstance()
    {
        static Renderer* _instance = new Renderer();
        return _instance;
    }

    void renderScene( Scene* scene );

private:
    Renderer();

    glm::mat4 m_perspectiveMatrix;
    glm::mat4 m_viewMatrix;

    void renderNode( Node* node, glm::mat4 currentGlobalMatrix );
};

#endif
