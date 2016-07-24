#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#define USE_PNG

namespace gre
{

class Texture
{
public:
static GLuint setTexture( const unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int channels=3 );

static void updateTexture( const GLuint& textureId, const unsigned char* data, const unsigned int width,
                    const unsigned int height, const unsigned int channels, const bool completeUpdate=false );

// Load a .BMP file using our custom loader
static GLuint loadBMP_custom(const char * imagepath);

// Load a .TGA file using GLFW's own loader
//static GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
static GLuint loadDDS(const char * imagepath);

#ifdef USE_PNG
static GLuint loadPNG(const char * imagepath);
#endif

};

}

#endif
