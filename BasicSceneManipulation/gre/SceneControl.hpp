#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <string>

// Define which interaction is used
enum InteractionType
{
	INTER_MOUSELOOK_KEYROTATE,
	INTER_MOUSELOOK_KEYTRANSLATE,
	INTER_MOUSENONE_KEYROTATE
};

class SceneControl
{
public:
	SceneControl();
	SceneControl(  InteractionType itype  );
	~SceneControl();

	// Singleton functions
	static SceneControl* getInstance();
	static void deleteInstance();

	void computeMatricesFromInputs();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void setControlType( InteractionType itype );
	InteractionType getControlType();

	void loadShaders( std::string vertexFileName, std::string fragmentFileName );
	unsigned int getProgram();
	void loadTextureDDS( std::string textureFileName );
	unsigned int getTexture();
	unsigned int getTextureID();
	unsigned int getMatrixID();

private:
	static SceneControl* sceneControlInstance;

	// Active interaction type
	InteractionType interactionType;

	// Calculated matrices
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 position;

	// View configuration
	// Initial horizontal angle : toward -Z
	float horizontalAngle;
	// Initial vertical angle : none
	float verticalAngle;
	// Initial Field of View
	float initialFoV;
	float transSpeed; // 3 units / second
	float rotSpeed;
	float mouseSpeed;

	// Scene modificators
	// Create and compile our GLSL program from the shaders
	unsigned int m_programID;
	// Get a handle for our "MVP" uniform
	unsigned int m_matrixID;
	// Load the texture
	unsigned int m_texture;
	// Get a handle for our "uSampler" uniform
	unsigned int m_textureID;

	void init();

	void computeMatricesMouseLookKeyRotate();
	void computeMatricesMouseNoneKeyRotate();
};

#endif
