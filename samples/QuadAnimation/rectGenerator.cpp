#include "rectGenerator.h"

#include "logger.h"
#define LOG_TAG "RectGenerator"

// Include GLM
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
    m_base->setTexture( m_uvtemplate );

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

    // Init Rectangles
    m_maxNumberOfRectangles = 10;
    m_lowWidth = 1;
    m_highWidth = 5;
    m_lowHeight = 2;
    m_highHeight = 5;
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
        float width = (float)math2d::randomNumberInterval(m_lowWidth, m_highWidth);
        float height = (float)math2d::randomNumberInterval(m_lowHeight, m_highHeight);
        Point2D origin = math2d::calculateRandomPoint2DInACircle(m_centerSquares, m_radiusSquares);
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

		gre::CustomObj* myQuad = createNodeQuad(name, origin.x, origin.y, width, height, 1.0f);
		m_objs.push_back(myQuad);
		m_scene.addChild(myQuad);
	}
	m_nodeQuadsGenerated = true;
	LOGI("Node quads generated!");
}

gre::CustomObj* RectGenerator::createNodeQuad(std::string name, float x0, float y0, float width, float height, float z)
{
	gre::CustomObj* customQuad;
	gre::Obj* quad = gre::ShapeDispatcher::getShapes()->getQuad();
	customQuad = new gre::CustomObj();
	customQuad->initWithObj(*quad);

	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	customQuad->setColor(r,g,b);

	customQuad->setName(name);
	customQuad->setShadersFromFiles( m_vShader, m_fColourShader );
	customQuad->setTexture( m_uvtemplate );

	customQuad->setTranslation(glm::vec3(x0, y0, z));
	customQuad->setScale(glm::vec3(width, height, 1.0f));

	return customQuad;
}
void RectGenerator::updateNodeQuads()
{
	if(!m_nodeQuadsGenerated)
	{
		LOGE("First generate node quads");
		return;
	}
	std::vector<gre::CustomObj*>::const_iterator it2 = m_objs.begin();
	for(std::vector<Square2D*>::const_iterator it = m_rectangles.begin(); it != m_rectangles.end(); it++)
	{
		Square2D* square = (*it);
		const Point2D origin = square->getOrigin();
		const float width = square->getWidth();
		const float height = square->getHeight();
		gre::CustomObj* obj = (*it2);
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

	for(std::vector<gre::CustomObj*>::const_iterator it = m_objs.begin(); it != m_objs.end(); ++it)
	{
		gre::CustomObj* obj = (*it);
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
