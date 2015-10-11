#include "rectGenerator.h"

#include "logger.h"
#define LOG_TAG "RectGenerator"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <typeinfo>

RectGenerator::RectGenerator():
m_quadsGenerated(false),
m_initialZoom(5.0f)
{
    m_assets_path = ASSET_DIRECTORY;

    m_vShader = m_assets_path+"shaders/basic130.vert";
    m_fShader = m_assets_path+"shaders/basic130.frag";
    m_fColourShader = m_assets_path+"shaders/colour.frag";
    //m_uvtemplate = m_assets_path+"obj/uvtemplate.DDS";
    m_uvtemplate = m_assets_path+"images/test.png";
    m_modelFile = m_assets_path+"obj/cube.obj";
}

RectGenerator::~RectGenerator()
{

}

bool RectGenerator::initScene()
{
    LOGI("Initializing scene...");
    // get renderer instance
    m_renderer = gre::Renderer::getInstance();

    // Generate the main model
    m_base = gre::ShapeDispatcher::getShapes()->getQuad();
    m_base->setName("Base");
    m_base->setShadersFromFiles( m_vShader, m_fShader );
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
    LOGI("Scene configured!");

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

bool RectGenerator::generateQuads()
{
    LOGI("Generating quads...");

    // Create random squares
    for(int i = 0; i < m_maxNumberOfRectangles; i++)
    {
        float width = (float)math2d::randomNumberInterval(m_lowWidth, m_highWidth);
        float height = (float)math2d::randomNumberInterval(m_lowHeight, m_highHeight);
        Point2D origin = math2d::calculateRandomPoint2DInACircle(m_centerSquares, m_radiusSquares);
        Square2D* square = new Square2D(origin, width, height);
        m_rectangles.push_back(square);
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

		gre::CustomObj* m_cube;
		m_cube = static_cast<gre::CustomObj*>(gre::ShapeDispatcher::getShapes()->getQuad());
		const int objID = m_objs.size();
		m_cube->setName("Quad" + std::to_string(objID));
		m_cube->setShadersFromFiles( m_vShader, m_fColourShader );
		m_cube->setTexture( m_uvtemplate );

		m_cube->setTranslation(glm::vec3(origin.x, origin.y, 1.0f));
		m_cube->setScale(glm::vec3(width, height, 1.0f));

		m_objs.push_back(m_cube);
		m_scene.addChild(m_cube);

	}
	LOGI("Node quads generated!");
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
