#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>

#define GL_GLEXT_PROTOTYPES
#include<GL/gl.h>
//#include<GLES2/gl2.h>

#include "shaderProgram.hpp"

using namespace std;
using namespace gre;

GLuint ProgramGenerator::makeProgramUsingFiles( const std::string& vertex_file_path, const std::string& fragment_file_path )
{
    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertex_file_path.c_str(), std::ios::in);
    if(vertexShaderStream.is_open()){
        std::string line = "";
        while(getline(vertexShaderStream, line))
            vertexShaderCode += "\n" + line;
        vertexShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path.c_str());
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
    if(fragmentShaderStream.is_open()){
        std::string line = "";
        while(getline(fragmentShaderStream, line))
            fragmentShaderCode += "\n" + line;
        fragmentShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path.c_str());
        return 0;
    }

    return makeProgram(vertexShaderCode, fragmentShaderCode);
}

GLuint ProgramGenerator::makeProgram( const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryCode)
{
	// Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

    GLint result = GL_FALSE;
    int infoLogLength;

	// Compile Vertex Shader
//	printf("Compiling shader : %s\n", vertexCode);
    char const * vertexSourcePointer = vertexCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
    glCompileShader(vertexShaderID);

	// Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        printf("%s\n", &vertexShaderErrorMessage[0]);
	}


	// Compile Fragment Shader
//	printf("Compiling shader : %s\n", fragmentCode);
    char const * fragmentSourcePointer = fragmentCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
    glCompileShader(fragmentShaderID);

	// Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
        printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);


    // Include geometry shader if needed
    bool geometryShaderAttached = false;
    if(static_cast<int>(geometryCode.size()) > 0)
    {
        // Compile Geometry Shader
    //	printf("Compiling shader : %s\n", fragmentCode);
        char const * geometrySourcePointer = geometryCode.c_str();
        glShaderSource(geometryShaderID, 1, &geometrySourcePointer , NULL);
        glCompileShader(geometryShaderID);

        // Check Fragment Shader
        glGetShaderiv(geometryShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(geometryShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if ( infoLogLength > 0 ){
            std::vector<char> geometryShaderErrorMessage(infoLogLength+1);
            glGetShaderInfoLog(geometryShaderID, infoLogLength, NULL, &geometryShaderErrorMessage[0]);
            printf("%s\n", &geometryShaderErrorMessage[0]);
        }
        glAttachShader(programID, geometryShaderID);
        geometryShaderAttached = true;
    }


    glLinkProgram(programID);

	// Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> programErrorMessage(infoLogLength+1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        printf("%s\n", &programErrorMessage[0]);
	}

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    if(geometryShaderAttached) glDeleteShader(geometryShaderID);


    return programID;
}


