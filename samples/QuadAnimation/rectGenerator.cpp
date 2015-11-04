#include "rectGenerator.h"

#include "logger.h"
#define LOG_TAG "RectGenerator"

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <unistd.h>

RectGenerator::RectGenerator():
m_quadsGenerated(false),
m_nodeQuadsGenerated(false),
m_initialZoom(1.0f),
m_background(NULL)
{
    m_assets_path = ASSET_DIRECTORY;

    m_gShader = m_assets_path+"shaders/basic.geom";
    m_vShader = m_assets_path+"shaders/basic130.vert";
    m_fShader = m_assets_path+"shaders/basic130.frag";
    m_fColourShader = m_assets_path+"shaders/colour.frag";
    m_fGradientShader = m_assets_path+"shaders/gradient.frag";
    m_fWaveShader = m_assets_path+"shaders/glowedWave.frag";
    //m_uvtemplate = m_assets_path+"obj/uvtemplate.DDS";
    m_uvtemplate = m_assets_path+"images/test.png";
    m_modelFile = m_assets_path+"obj/cube.obj";

    m_gShaderTron = m_assets_path+"shaders/basicTron.geom";
    m_vShaderTron = m_assets_path+"shaders/basicTron.vert";
    m_fShaderTron = m_assets_path+"shaders/basicTron.frag";

    m_rectangles.clear();
    m_rooms.clear();
    m_objs.clear();
}

RectGenerator::~RectGenerator()
{
	if(m_background)
	{
		delete m_background;
	}
	// Free memory
	destroyNodeQuads();
	destroyQuads();
}

bool RectGenerator::initScene()
{
    LOGI("Initializing scene...");
    // get renderer instance
    m_renderer = gre::Renderer::getInstance();

    // Generate the main model
    m_base = gre::ShapeDispatcher::getShapes()->getQuad();
    m_base->setName("Base");
    m_base->setShadersFromFiles( m_vShader, m_fGradientShader );

    // Generate camera instance
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    glm::vec3 up = glm::vec3( 0,1,0 );

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //m_camera.setConfiguration(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
    m_camera.setConfiguration(5.f, 0.1f, 1000.0f);
    // View matrix
    m_camera.setLocation( position,           // Camera is here
            glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Generate scene
    m_scene.setName("Scene");
    m_scene.addCamera(m_camera);
    m_scene.addChild(m_base);
    LOGI("Scene initialized!");

    // Configuration quads algorithm
    m_maxNumberOfRectangles = 2;
    m_lowWidth = 7;
    m_highWidth = 7;
    m_lowHeight = 7;
    m_highHeight = 7;
    m_roomSize = 3;
    m_centerSquares = Point2D(0.0f, 0.0f);
    m_radiusSquares = 5.0f;
    m_initialZoom = 15.0f;
    LOGI("Scene configured!");

    // Update zoom
    zoom(m_initialZoom);
    return true;
}

bool RectGenerator::updateScene()
{
	// Your code here.
    return glfwWindowShouldClose(m_window);
}

bool RectGenerator::render()
{
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //scene.draw();
   m_renderer->renderScene(&m_scene);

   // Swap buffers
   glfwSwapBuffers(m_window);
   glfwPollEvents();
}

void RectGenerator::close()
{
    LOGI("Freeing scene memory..");
    for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
    {
        delete *it;
    }
    m_rectangles.clear();

    GLFWWindowImpl::close();
}

void RectGenerator::printQuadsInfo()
{
	if(!m_quadsGenerated)
	{
		LOGE("First generate quads");
		return;
	}
	// Print info
	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		std::string data = (*it)->getJSONFormat();
		LOGD("Data: %s", data.c_str());
	}
	return;
}

bool RectGenerator::generateQuads(bool renderEach, unsigned int delayMs)
{
    LOGI("Generating quads...");

    if(renderEach)
    {
    	// Pre-render
		createNodeQuads();
		render();
		destroyNodeQuads();
		usleep(delayMs*1000);
    }

    // Create random squares
    for(int i = 0; i < m_maxNumberOfRectangles; i++)
    {
    	// Random W & H
        float width = (float)math2d::randomNumberInterval(m_lowWidth, m_highWidth);
        float height = (float)math2d::randomNumberInterval(m_lowHeight, m_highHeight);

        LOGD("Quad with size: %fx%f", width, height);

        // Generate int origin points
        Point2D origin = math2d::calculateRandomPoint2DInACircle(m_centerSquares, m_radiusSquares);
        origin.x = (int)origin.x;
        origin.y = (int)origin.y;

        // Create abstract squares
        Square2D* square = new Square2D(origin, width, height);
        m_rectangles.push_back(square);

        if(renderEach)
        {
        	createNodeQuads();
        	render();
        	destroyNodeQuads();
        	usleep(delayMs*1000);
        }
    }
    m_quadsGenerated = true;
    LOGI("Quads generated!");
    return true;
}

bool RectGenerator::separateQuads(bool renderEach, unsigned int delayMs)	// Muy costoso, probar otra estrategia, cuando haya solapamiento no parar hasta separarlo
{
	LOGI("Separating node quads...");

	bool existOverlap = true;

	while(existOverlap)
	{
		// Assume no overlap
		existOverlap = false;

		for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
		{
			// Get quads overlap except itself
			std::vector<Square2D*> overlapList = quadOverlapWith((*it));
			LOGD("Overlaps with: %d", overlapList.size());
			if(overlapList.size() > 0)
			{
				// Will end if no overlap for each one
				existOverlap = true;
				for(std::vector<Square2D*>::const_iterator it2 = overlapList.begin(); it2 != overlapList.end(); it2++)
				{
					Point2D centerPoint1 = (*it)->getCenter();
					Point2D centerPoint2 = (*it2)->getCenter();
					Point2D originPoint1 = (*it)->getOrigin();
					Point2D originPoint2 = (*it2)->getOrigin();
					float width1 = (*it)->getWidth();
					float width2 = (*it2)->getWidth();
/*
					const float deltaCenterX = abs(centerPoint1.x - centerPoint2.x);
					const float deltaWidth = width2 - width1;
					if(deltaWidth - deltaCenterX < 0)
					{
						centerPoint1.x -= 1;
                        (*it)->setOrigin(centerPoint1); // (goe): Error de compilación
					}
					else if(deltaWidth - deltaCenterX > 0)
					{
						centerPoint1.x += 1;
                        (*it)->setOrigin(centerPoint1); // (goe): Error de compilación
					}
					else
					{
						LOGD("Aw right");
					}
					*/
					const float deltaCenterX = centerPoint2.x - centerPoint1.x;
					const float limit = width2 + width1;
					LOGD("Data deltaCenterX: %f, limit: %f", deltaCenterX, limit);
					if(deltaCenterX < limit)
					{
						originPoint1.x -= 1;
						(*it)->setOrigin(originPoint1);
					}
					else if(deltaCenterX > limit)
					{
						originPoint1.x += 1;
						(*it)->setOrigin(originPoint1);
					}
					else
					{
						LOGD("Done!");
					}
					if(renderEach)
					{
						updateNodeQuads();
						// Pre-render
						//createNodeQuads();
						render();
						//destroyNodeQuads();
						usleep(delayMs*1000);
					}
				}
			}


		}
	}

	LOGI("Node quads separated!");
	return true;
}

bool RectGenerator::createHallways()
{
	LOGI("Generating hallways...");
	LOGI("Hallways generated!");
	return true;
}

bool RectGenerator::quadOverlap(Square2D* square1, Square2D* square2)
{
	const float deltaCenterX = square1->getCenter().x - square2->getCenter().x ;
	const float deltaWidth = square2->getWidth() + square1->getWidth();

	LOGD("quadOverlap: %f, %f", deltaCenterX, deltaWidth);
	if(deltaCenterX > deltaWidth)
	{
		LOGD("FALSE");
		return false;
	}
	else
	{
		LOGD("TRUE");
		return true;
	}
}

std::vector<Square2D*> RectGenerator::quadOverlapWith(Square2D* square)
{
	std::vector<Square2D*> overlapList;

	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		if((*it) == square)
		{
			//LOGD("Avoid me :D");
			break;
		}
		else
		{
			if(quadOverlap((*it), square))
			{
				overlapList.push_back((*it));
			}
		}
	}
	return overlapList;
}

bool RectGenerator::selectRooms()
{
	if(!m_quadsGenerated)
	{
		LOGE("First generate quads");
		return false;
	}
	m_rooms.clear();	// Copy quads pointers, never mind if remove old references

	LOGI("Selecting rooms...");
	// Select
	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		Square2D* square = (*it);
		const Point2D origin = square->getOrigin();
		const float width = square->getWidth();
		const float height = square->getHeight();
		if(width >= m_roomSize && height >= m_roomSize)
		{
			//LOGD("It`s a room");
			m_rooms.push_back(square);
		}
	}
	LOGI("There are %d rooms in total of %d", (int)m_rooms.size(), (int)m_rectangles.size());
	return true;
}

bool RectGenerator::createRoomGraph()
{
	if(m_rooms.size() <= 0)
	{
		LOGE("First generate rooms");
		return false;
	}

	LOGI("Creating graph with rooms centers...");
	LOGI("Graph created!");
}

bool RectGenerator::createConnections()
{
	LOGI("Creating connections between rooms");
	LOGI("Connections created!");
}

bool RectGenerator::addLoops()
{
	LOGI("Adding loops");
	LOGI("Loops insertion completed!");
}

bool RectGenerator::createCorridors()
{
	LOGI("Creating corridors...");
	LOGI("Corridors created!");
}
void RectGenerator::createNodeQuads()
{
	LOGI("Generating node quads...");

	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		Square2D* square = (*it);
		const Point2D origin = square->getOrigin();
		const float width = square->getWidth();
		const float height = square->getHeight();
		const int objID = m_objs.size();
		std::string name = "Quad" + std::to_string(objID);

        gre::Obj* myQuad = createNodeQuad(name, origin.x, origin.y, width, height, 1.0f);
		m_objs.push_back(myQuad);
		m_scene.addChild(myQuad);
	}
	m_nodeQuadsGenerated = true;
	LOGI("Node quads generated!");
}

float RectGenerator::getRandomNumberBetween0and1()
{
    static bool isInit = false;
    if(!isInit)
    {
        srand(time(NULL));
        isInit = true;
    }

    return ((float) rand() / (RAND_MAX));
}

gre::Obj* RectGenerator::createNodeQuad(std::string name, float x0, float y0, float width, float height, float z)
{
    gre::Obj* quad = gre::ShapeDispatcher::getShapes()->getQuad();

    // Generate colour uniform structure
    std::vector<std::string> extraShaderUniforms;
    std::vector<std::vector<float> > extraShaderUniformValues;
    extraShaderUniforms.push_back("iColour");
    std::vector<float> color(3);
    color[0] = getRandomNumberBetween0and1();  // red
    color[1] = getRandomNumberBetween0and1();  // green
    color[2] = getRandomNumberBetween0and1();  // blue
    LOGI("Generated colour [%f,%f,%f]\n", color[0], color[1], color[2]);
    extraShaderUniformValues.push_back(color);

    quad->setName(name);
    quad->setShadersFromFiles( m_vShader, m_fColourShader, extraShaderUniforms );
    quad->setExtraValues(extraShaderUniformValues);

    quad->setTranslation(glm::vec3(x0, y0, z));
    quad->setScale(glm::vec3(width, height, 1.0f));

    return quad;
}

void RectGenerator::updateNodeQuads()
{
	if(!m_nodeQuadsGenerated)
	{
		LOGE("First generate node quads");
		return;
	}
    std::vector<gre::Obj*>::const_iterator it2 = m_objs.begin();
	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		Square2D* square = (*it);
		const Point2D origin = square->getOrigin();
		const float width = square->getWidth();
		const float height = square->getHeight();
        gre::Obj* obj = (*it2);
		obj->setTranslation(glm::vec3(origin.x, origin.y, 1.0f));
		obj->setScale(glm::vec3(width, height, 1.0f));
		++it2;
	}
}

void RectGenerator::destroyQuads()
{
	LOGI("Destroying Quads...");

	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		delete (*it);
	}
	m_rectangles.clear();
	LOGI("Quads destroyed!");
}

void RectGenerator::destroyNodeQuads()
{
	LOGI("Destroying node quads...");

    for(std::vector<gre::Obj*>::const_iterator it = m_objs.begin(); it != m_objs.end(); ++it)
	{
        gre::Obj* obj = (*it);
		m_scene.removeChild(obj);
	}
	m_objs.clear();
	LOGI("Node quads destroyed!");
}

void RectGenerator::createBackgroundQuad()
{
	LOGI("Creating background quad...");

	float minX = 0.0f;
	float minY = 0.0f;
	float maxX = 0.0f;
	float maxY = 0.0f;
	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		Square2D* square = (*it);
		const Point2D origin = square->getOrigin();
		const float width = square->getWidth();
		const float height = square->getHeight();
		if(origin.x < minX) minX = origin.x;
		if(origin.y < minY) minY = origin.y;
		if(origin.x + width > maxX) maxX = origin.x + width;
		if(origin.y + height > maxY) maxY = origin.y + height;
	}

	LOGD("Min x is: %f", minX);
	LOGD("Min y is: %f", minY);
	LOGD("Max x is: %f", maxX);
	LOGD("Max y is: %f", maxY);
	m_background = createNodeQuad("Background", minX, minY, maxX, maxY, 1.0f);
}
void RectGenerator::getCameraPosition(glm::vec3& pos)
{
    glm::mat4 transMat = m_camera.getGlobalTranslation();
}

void RectGenerator::setCameraPosition(glm::vec3& pos)
{
    glm::mat4 transMat = m_camera.getGlobalTranslation();
}

void RectGenerator::translateCameraPosition(const glm::vec3& offset)
{
    glm::vec3 up = m_camera.getUp();
    glm::vec3 location = m_camera.getLocation();
    glm::vec3 target = m_camera.getTarget();

    location += offset;
    target += offset;

    m_camera.setLocation(location, target, up);
}

void RectGenerator::zoom(const float value)
{
    m_camera.setConfiguration( -value,
                                value,
                               -value,
                                value,
                                m_camera.getNearValue(),
                                m_camera.getFarValue());
}
