#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "SceneControl.hpp"

#include "shaderProgram.hpp"
#include "texture.hpp"

SceneControl* SceneControl::sceneControlInstance = NULL;

SceneControl* SceneControl::getInstance()
{
	if( sceneControlInstance == NULL )
	{
		sceneControlInstance = new SceneControl();
	}
	return sceneControlInstance;
}

void SceneControl::deleteInstance()
{
	if( sceneControlInstance != NULL )
	{
		delete sceneControlInstance;
	}
}

glm::mat4 SceneControl::getViewMatrix()
{
	return ViewMatrix;
}
glm::mat4 SceneControl::getProjectionMatrix()
{
	return ProjectionMatrix;
}

SceneControl::SceneControl()
{
	init();
	setControlType( INTER_MOUSELOOK_KEYTRANSLATE );
}

SceneControl::SceneControl( InteractionType itype )
{
	init();
	setControlType( itype );
}

SceneControl::~SceneControl()
{
	if( m_programID != 0 )
		glDeleteProgram(m_programID);

	if( m_textureID != 0 )
		glDeleteTextures(1, &m_textureID);
}

void SceneControl::init()
{
	// Initial position : on +Z
	position = glm::vec3( 0, 0, 5 );
	// Initial horizontal angle : toward -Z
	horizontalAngle = 3.14f;
	// Initial vertical angle : none
	verticalAngle = 0.0f;
	// Initial Field of View
	initialFoV = 45.0f;

	transSpeed = 3.0f; // 3 units / second
	rotSpeed = 1.0f;
	mouseSpeed = 0.005f;

	m_programID = 0;
	m_textureID = 0;
}

void SceneControl::loadShaders( std::string vertexFileName, std::string fragmentFileName )
{
    m_programID = ProgramGenerator::makeProgramUsingFiles( vertexFileName, fragmentFileName );
	m_matrixID = glGetUniformLocation(m_programID, "MVP");
	m_textureID  = glGetUniformLocation(m_programID, "uSampler");
}

unsigned int SceneControl::getProgram()
{
	return m_programID;
}

void SceneControl::loadTextureDDS( std::string textureFileName )
{
	m_texture = loadDDS(textureFileName.c_str());
}

unsigned int SceneControl::getTexture()
{
	return m_texture;
}

unsigned int SceneControl::getTextureID()
{
	return m_textureID;
}

unsigned int SceneControl::getMatrixID()
{
	return m_matrixID;
}

void SceneControl::setControlType( InteractionType itype )
{
	interactionType = itype;
}

InteractionType SceneControl::getControlType()
{
	return interactionType;
}


void SceneControl::computeMatricesFromInputs()
{
	switch( interactionType )
	{
		case INTER_MOUSELOOK_KEYTRANSLATE:
			computeMatricesMouseLookKeyRotate();
			break;
		case INTER_MOUSENONE_KEYROTATE:
			computeMatricesMouseNoneKeyRotate();
			break;
		case INTER_MOUSELOOK_KEYROTATE:
			break;
		default:
			computeMatricesMouseLookKeyRotate();
	}
}

void SceneControl::computeMatricesMouseLookKeyRotate()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	int xpos, ypos;
	glfwGetMousePos(&xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetMousePos(1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * transSpeed;
	}
	// Move backward
	if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * transSpeed;
	}
	// Strafe right
	if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * transSpeed;
	}
	// Strafe left
	if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * transSpeed;
	}

	float FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void SceneControl::computeMatricesMouseNoneKeyRotate()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Strafe right
	if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS)
	{
		horizontalAngle -= horizontalAngle * deltaTime * rotSpeed;
	}
	// Strafe left
	if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS)
	{
		horizontalAngle += horizontalAngle * deltaTime * rotSpeed;
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS)
	{
		position += direction * deltaTime * transSpeed;
	}
	// Move backward
	if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS)
	{
		position -= direction * deltaTime * transSpeed;
	}

	float FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
