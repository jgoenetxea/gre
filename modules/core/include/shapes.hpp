/*
 * shapes.hpp
 *
 *  Created on: Apr 7, 2013
 *      Author: goe
 */

#ifndef SHAPES_HPP_
#define SHAPES_HPP_

#include"obj.hpp"

namespace gre
{

static const float SHAPE_CUBE_VERTEX[] =
{
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f
};

// Two UV coordinates for each vertex. They were created with Blender.
static const float SHAPE_CUBE_UV[] =
{
	0.000059f, 0.000004f,
	0.000103f, 0.336048f,
	0.335973f, 0.335903f,
	1.000023f, 0.000013f,
	0.667979f, 0.335851f,
	0.999958f, 0.336064f,
	0.667979f, 0.335851f,
	0.336024f, 0.671877f,
	0.667969f, 0.671889f,
	1.000023f, 0.000013f,
	0.668104f, 0.000013f,
	0.667979f, 0.335851f,
	0.000059f, 0.000004f,
	0.335973f, 0.335903f,
	0.336098f, 0.000071f,
	0.667979f, 0.335851f,
	0.335973f, 0.335903f,
	0.336024f, 0.671877f,
	1.000004f, 0.671847f,
	0.999958f, 0.336064f,
	0.667979f, 0.335851f,
	0.668104f, 0.000013f,
	0.335973f, 0.335903f,
	0.667979f, 0.335851f,
	0.335973f, 0.335903f,
	0.668104f, 0.000013f,
	0.336098f, 0.000071f,
	0.000103f, 0.336048f,
	0.000004f, 0.671870f,
	0.336024f, 0.671877f,
	0.000103f, 0.336048f,
	0.336024f, 0.671877f,
	0.335973f, 0.335903f,
	0.667969f, 0.671889f,
	1.000004f, 0.671847f,
	0.667979f, 0.335851f
};

class Shapes
{
public:
	Shapes();
	virtual ~Shapes();

    Obj* getCube();

    Obj* getQuad();
};



//***********************************************************/
// 				SHAPE DISPATCHER (Singleton)
//***********************************************************/
class ShapeDispatcher
{
public:
	static Shapes* getShapes()
	{
        static Shapes* shapesSingleton = new Shapes();
		return shapesSingleton;
	}

	static void destroyShapes()
	{
	}
};

}
#endif /* SHAPES_HPP_ */
