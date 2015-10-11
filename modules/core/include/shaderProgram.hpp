#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

namespace gre
{

class ProgramGenerator
{
public:
    static GLuint makeProgram( const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryCode="");

    static GLuint makeProgramUsingFiles( const std::string& vertex_file_path, const std::string& fragment_file_path);
};

}

#endif
