#include "scene.hpp"

Scene::Scene():m_currentCamera(-1)
{

}

Scene::~Scene()
{

}

unsigned int Scene::addCamera( Camera& c )
{
    m_currentCamera = static_cast<int>( m_cameraList.size() );
    m_cameraList.push_back(&c);

    return static_cast<unsigned int>(m_currentCamera);
}

void Scene::setCurrentCamera( const unsigned int id )
{
    if( id < static_cast<unsigned int>( m_cameraList.size() ) )
    {
        m_currentCamera = id;
    }
}

Camera* Scene::getCamera( const unsigned int id )
{
    if( id < static_cast<unsigned int>( m_cameraList.size() ) )
    {
        return m_cameraList[id];
    }
    return NULL;
}
