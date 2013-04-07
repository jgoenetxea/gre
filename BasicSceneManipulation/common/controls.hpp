#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// Define which interaction is used
enum InteractionType
{
	INTER_MOUSELOOK_KEYROTATE,
	INTER_MOUSELOOK_KEYTRANSLATE,
	INTER_MOUSENONE_KEYROTATE
};

class SceneControls
{
public:
	SceneControls();
	SceneControls(  InteractionType itype  );
	~SceneControls();
	void computeMatricesFromInputs();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void setControlType( InteractionType itype );
	InteractionType getControlType();
private:
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

	void init();

	void computeMatricesMouseLookKeyRotate();
	void computeMatricesMouseNoneKeyRotate();
};
#endif
