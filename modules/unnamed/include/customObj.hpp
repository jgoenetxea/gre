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
    void setShaders( const string& vertexShaderCode, const string& fragmentShaderCode, const string& geometryShaderCode="" );
    void setShadersFromFiles( const std::string& vertex_file_path, const std::string& fragment_file_path, const std::string& geometry_file_path="" );
    void updateFragmentShader( const string& fragmentShaderCode );

    void initWithObj(Obj& obj); // move assignment

protected:
    unsigned int m_colourUniformLocator;
};

}

#endif
