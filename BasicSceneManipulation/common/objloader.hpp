#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "obj.hpp"
#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>

class ObjFactory
{
public:
	ObjFactory();
	~ObjFactory();

	static ObjFactory* getInstance();
	static void deleteInstance();

	static Obj* loadOBJ( std::string filename );
	static int isEqualStored( 	std::vector<unsigned short>& vertexIndices,
									std::vector<unsigned short>& uvIndices,
									std::vector<unsigned short>& normalIndices,
									unsigned int currentIndex
									);
};

#endif
