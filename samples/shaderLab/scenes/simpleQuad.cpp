#include "simpleQuad.h"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>

using namespace glm;
SimpleQuad::SimpleQuad()
{
    m_frame = 0;

    m_assets_path = ASSET_DIRECTORY;

    m_vShader = m_assets_path+"shaders/basic130.vert";
    m_fShader = m_assets_path+"shaders/basic130.frag";
    m_fColourShader = m_assets_path+"shaders/colour.frag";
    //m_uvtemplate = m_assets_path+"obj/uvtemplate.DDS";
    m_uvtemplate = m_assets_path+"images/test.png";
    m_modelFile = m_assets_path+"obj/cube.obj";
}

SimpleQuad::~SimpleQuad()
{

}

void SimpleQuad::initialize()
{
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // Not used if only one vertes array is used. If more than one vertex array is used, try with 'glVertexAttribPointer()'.
    //glGenVertexArrays(1, &VertexArrayID);
    //glBindVertexArray(VertexArrayID);

    // get renderer instance
    m_renderer = gre::Renderer::getInstance();

    // Generate the main model
    m_obj = gre::ShapeDispatcher::getShapes()->getQuad();
    m_obj->setShadersFromFiles( m_vShader, m_fShader );
    //m_obj->setTexture( m_uvtemplate );

    // Generate camera instance
    position = glm::vec3( 0, 0, 5 );
    up = glm::vec3( 0,1,0 );


    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_camera.setConfiguration(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);
    // View matrix
    m_camera.setLocation( position,           // Camera is here
                          glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
                          up                  // Head is up (set to 0,-1,0 to look upside-down)
                          );

    // Generate scene
    m_scene.addCamera(m_camera);
    m_scene.addChild(&m_trans);
    m_trans.addChild(m_obj);

    // Initial position : on +Z
    m_timer = new gre::Timer();
    horizontalAngle = 0.f;
    translateValue = 0.f;
    rotSpeed = 80;
    transSpeed = 14;

    m_frame = 0;
}

void SimpleQuad::paint()
{
    // time control
    float deltaTime = m_timer->splitSeconds();
    //float deltaTime = float(currentTime - lastTime);
    //lastTime = currentTime;

    //std::cout << "Delta: " << deltaTime << std::endl;

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    horizontalAngle += deltaTime * rotSpeed;
    translateValue += deltaTime * transSpeed;

    glm::mat4 ModelMatrix = glm::mat4(1.0);
    //ModelMatrix = glm::rotate(ModelMatrix, horizontalAngle, glm::vec3(1, 0, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)

    m_trans.setLocalTranslation(ModelMatrix);

    m_renderer->renderScene(&m_scene);
    //std::cout << "Render cube #" << m_frame << std::endl;
    m_frame++;
}

void SimpleQuad::updateFragmentShader( const string& fragmentShaderCode )
{
    m_obj->updateFragmentShader( fragmentShaderCode );
}

std::string& SimpleQuad::getCurrentFragmentShaderCode()
{
    return m_obj->getCurrentFragmentShaderCode();
}

std::string& SimpleQuad::getCurrentVertexShaderCode()
{
    return m_obj->getCurrentVertexShaderCode();
}
