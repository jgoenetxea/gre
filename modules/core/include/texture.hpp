#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#define USE_PNG

namespace gre
{

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

#ifdef USE_PNG
GLuint loadPNG(const char * imagepath);
bool loadPngImage(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);
#endif

}

#endif
