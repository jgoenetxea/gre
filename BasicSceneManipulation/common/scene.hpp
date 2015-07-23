#ifndef SCENE_H
#define SCENE_H

#include "node.hpp"
#include "camera.hpp"

#include <vector>
#include <string>

using namespace std;

class Scene : public Node
{
public:
    Scene();
    ~Scene();

    unsigned int addCamera( Camera& c );
    void setCurrentCamera( const unsigned int id );
    Camera* getCamera( const unsigned int id );
    Camera* getCurrentCamera();

private:
    std::vector<Camera*> m_cameraList;
    int m_currentCamera;
};

#endif
