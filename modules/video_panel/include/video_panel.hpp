#ifndef GRE_SCREEN_PANEL_H
#define GRE_SCREEN_PANEL_H

//#include "scene.hpp"
//#include "renderer.hpp"
#include "obj.hpp"
#include "node.hpp"

namespace gre {

class VideoPanel : public Node
{
public:
    VideoPanel();
    ~VideoPanel();

    void draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective );

    void setFrame( const unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int channels );

private:

    void initialize();

    //gre::Scene m_scene;
    //gre::Renderer* m_renderer;
    gre::Obj* m_obj;

    unsigned int m_width;
    unsigned int m_height;
};

}
#endif  // GRE_SCREEN_PANEL_H
