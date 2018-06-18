#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PhysicsEngine.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP
};

static const GLfloat __INIT_YAW						= -90.0f;
static const GLfloat __INIT_PITCH					= 0.0f;
static const GLfloat __INIT_MOUSE_SENSITIVITY		= 0.7f;
static const GLfloat __INIT_SCROLL_SENSITIVITY		= 0.1f;
static const GLfloat __INIT_FOV						= 45.0f;

static const GLfloat __MAX_PITCH					= 89.0f;
static const GLfloat __MIN_PITCH					= -89.0f;
static const GLfloat __MAX_FOV						= 45.0f;
static const GLfloat __MIN_FOV						= 1.0f;

class Camera
{
public:
	// Physics Engine
	PhysicsEngine* physicsEngine;
private:
	
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 TargetPos;// movement target position
	// hero body
	glm::vec3 BodyFront;
	glm::vec3 BodyRight;
	glm::vec3 BodyUp;
	// hero head and eyes
	glm::vec3 EyeFront;
	glm::vec3 Up;
	glm::vec3 Right;
	// Euler Angles (ignore Roll)
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat ScrollSensitivity;
	GLfloat Fov;

	// initial or calculate the front vector after update the Euler Angle.
	GLvoid UpdateCameraVectors();
public:
	// Constructor function and destructor function
	Camera(glm::vec3 position,
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = __INIT_YAW, GLfloat pitch = __INIT_PITCH,
		GLfloat movementSpeed = MoveSpeed, GLfloat mouseSensitivity = __INIT_MOUSE_SENSITIVITY,
		GLfloat scrollSensitivity = __INIT_SCROLL_SENSITIVITY, GLfloat fov = __INIT_FOV);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, 
		GLfloat upX = 0.0f, GLfloat upY = 1.0f, GLfloat upZ = 0.0f, 
		GLfloat yaw = __INIT_YAW, GLfloat pitch = __INIT_PITCH,
		GLfloat movementSpeed = MoveSpeed, GLfloat mouseSensitivity = __INIT_MOUSE_SENSITIVITY, 
		GLfloat scrollSensitivity = __INIT_SCROLL_SENSITIVITY, GLfloat fov = __INIT_FOV);
	Camera();
	~Camera();

	// Update the camera's position corresponding to the keyboard event.
	// deltatime just to eliminate the performance among different PCs.
	GLvoid ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);	

	// Update the camera's Euler angle corresponding to the mouse move event.
	GLvoid ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset);

	// Update the camera's zoom corresponding to the mouse scroll event.
	GLvoid ProcessMouseScroll(GLfloat yoffset);
	
	// Loop and update data
	GLvoid LoopFunction(GLfloat deltaTime);

	// Get the camera's corresponding view Matrix
	const glm::mat4 GetViewMatrix();
	
	// Get Camera's Attributes
	const glm::vec3& GetPosition();
	const glm::vec3& GetTargetPos();
	const glm::vec3& GetEyeFront();
	const glm::vec3& GetBodyFront();
	const glm::vec3& GetBodyRight();
	const glm::vec3& GetBodyUp();
	const glm::vec3& GetUp();
	const glm::vec3& GetRight();
	
	// Get Camera's Euler Angle
	const GLfloat GetYaw();
	const GLfloat GetPitch();

	// Get Camera's options
	const GLfloat GetMovementSpeed();
	const GLfloat GetMouseSensitivity();
	const GLfloat GetScrollSensitivity();
	const GLfloat GetFov();

	// Set Physics Engine
	GLvoid SetOuterBound(const glm::vec4 &outBound);
	GLvoid SetinnerBound(const glm::vec3 &b1,const glm::vec3 &b2);

	// Set Camera's Attributes
	GLvoid SetPosition(const glm::vec3 &position);
	GLvoid SetTargetPos(const glm::vec3 &position);
	
	GLvoid SetBodyFront(const glm::vec3 &front);
	GLvoid SetBodyRight(const glm::vec3 &front);
	GLvoid SetBodyUp(const glm::vec3 &front);
	
	GLvoid SetEyeFront(const glm::vec3 &front);
	GLvoid SetUp(const glm::vec3 &up);
	GLvoid SetRight(const glm::vec3 &right);

	// Set Camera's Euler Angle
	GLvoid SetYaw(const GLfloat &yaw);
	GLvoid SetPitch(const GLfloat &Pitch);

	// Set Camera's options
	GLvoid SetMovementSpeed(const GLfloat &movementSpeed);
	GLvoid SetMouseSensitivity(const GLfloat &mouseSensitivity);
	GLvoid SetScrollSensitivity(const GLfloat &scrollSensitivity);
	GLvoid SetFov(const GLfloat &fov);
};

