#include <stdio.h>
#include <string.h>

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>

#include "video_panel.hpp"

#include "shapes.hpp"

#include "texture.hpp"

//const std::string uvtemplate = "/home/goe/Repositorios/git/labirinth/samples/BasicSceneManipulation/assets/obj/cube.png";
//const std::string vShader = "/home/goe/Repositorios/git/labirinth/samples/BasicSceneManipulation/assets/shaders/basic130.vert";
//const std::string fShader = "/home/goe/Repositorios/git/labirinth/samples/BasicSceneManipulation/assets/shaders/basic130.frag";

const std::string c_screenPanelVertexShader = " \
        #version 130 \n \
         \
        // Input vertex data, different for all executions of this shader. \n \
        in vec3 vertexPosition_modelspace; \n \
        in vec2 vertexUV; \n \
        in vec3 vertexNormal; \n \
        \n  \
        // Output data ; will be interpolated for each fragment. \n \
        out vec2 UV; \n \
        \n  \
        // Values that stay constant for the whole mesh. \n \
        uniform mat4 modelViewMatrix; \n \
        \n  \
        void main(){ \n \
        \n  \
            // Output position of the vertex, in clip space : MVP * position \n \
            gl_Position =  modelViewMatrix * vec4(vertexPosition_modelspace,1); \n \
            \n  \
            // UV of the vertex. No special space for this one.\n  \
            UV = vertexUV; \n \
        }\n ";

const std::string c_screenPanelFragmentShader = "\
        #version 130 \n \
        \n \
        // Interpolated values from the vertex shaders\n \
        in vec2 UV; \n \
        \n \
        // Ouput data \n \
        out vec3 fragColor;\n \
         \n \
        // Values that stay constant for the whole mesh. \n \
        uniform sampler2D iChannel0; \n \
        uniform vec3 iResolution;		// Viewport resolution (in pixels) \n \
        uniform float iGlobalTime;		// shader playback time (in seconds) \n \
        uniform vec4 iMouse;			// mouse pixel coords. xy: current (if MLB down), zw: click \n \
        uniform vec4 iDate;				// (year, month, day, time in seconds) \n \
        uniform float iSampleRate;		// sound sample rate (i.e., 44100) \n \
        \n  \
        void main(){ \n \
        \n  \
            // Output color = color of the texture at the specified UV \n \
            fragColor = texture2D( iChannel0, UV ).rgb; \n \
            //fragColor = vec3(255,0,0); \n \
        }\n ";

static const unsigned int emptyTextureWidth = 320;
static const unsigned int emptyTextureHeight = 240;
static const unsigned int emptyTextureDepth = 3;
static unsigned char* emptyTextureData = (unsigned char*)malloc(emptyTextureWidth * emptyTextureHeight * emptyTextureDepth);

namespace gre
{

VideoPanel::VideoPanel() : m_width(0)
                         , m_height(0)
{
    // Initialize the panel
    initialize();
}

VideoPanel::~VideoPanel()
{

}

void VideoPanel::initialize()
{
    // Generate the main model
    std::vector<std::string> emptyValues;
    m_obj = gre::ShapeDispatcher::getShapes()->getQuad();
    m_obj->setShaders( c_screenPanelVertexShader, c_screenPanelFragmentShader, "", emptyValues );
    //m_obj->setTexture( uvtemplate );
    m_obj->setName("video_panel");

    // Init the texture as black image
    memset(emptyTextureData, 127, emptyTextureWidth * emptyTextureHeight * emptyTextureDepth);
    // Init the texture structure
    GLuint texture = Texture::setTexture( emptyTextureData, emptyTextureWidth, emptyTextureHeight, emptyTextureDepth );
    m_obj->setTexture(texture);
    // Set the frame as texture
    setFrame( emptyTextureData, emptyTextureWidth, emptyTextureHeight, emptyTextureDepth );
}

void VideoPanel::draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective )
{
    //m_renderer->renderScene(&m_scene);
    m_obj->draw(model, view, perspective);
}

void VideoPanel::setFrame( const unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int channels )
{
    GLuint objTexture = m_obj->getTextureId();

    if(width != m_width || height != m_height)
    {
        // Renew texture data
        m_width = width;
        m_height = height;

        //m_obj->setScale(glm::vec3(width, height, 1.f));

        Texture::updateTexture( objTexture, data, m_width, m_height, channels, true );
    }
    else
    {
        Texture::updateTexture( objTexture, data, m_width, m_height, channels, false );
    }
}

}
