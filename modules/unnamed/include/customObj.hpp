#ifndef CUSTOMOBJ_H
#define CUSTOMOBJ_H

#include "obj.hpp"
#include "timer.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>

using namespace std;

namespace gre
{

class CustomObj: public Obj
{
public:
    CustomObj();
    ~CustomObj();

    void draw( const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective );
    void setShaders( const string& vertexShaderCode, const string& fragmentShaderCode );
    void setShadersFromFiles( std::string& vertex_file_path, std::string& fragment_file_path );
    void updateFragmentShader( const string& fragmentShaderCode );

    CustomObj& operator=(Obj& obj); // move assignment

};

}

#endif
