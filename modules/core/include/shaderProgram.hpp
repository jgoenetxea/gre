#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

namespace gre
{

class ProgramGenerator
{
public:
    static GLuint makeProgram( const std::string& vertex_file_path, const std::string& fragment_file_path);

    static GLuint makeProgramUsingFiles( const std::string& vertex_file_path, const std::string& fragment_file_path);
};

}

#endif
