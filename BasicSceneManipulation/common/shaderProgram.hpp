#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

namespace gre
{

class ProgramGenerator
{
public:
    static GLuint makeProgram( std::string& vertex_file_path, std::string& fragment_file_path);

    static GLuint makeProgramUsingFiles(std::string& vertex_file_path, std::string& fragment_file_path);
};

}

#endif
