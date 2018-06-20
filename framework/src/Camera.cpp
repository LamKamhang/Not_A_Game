#include "Header/Camera.h"


// Constructor function and destructor function
Camera::Camera(glm::vec3 position, glm::vec3 up, 
				GLfloat yaw, GLfloat pitch,
				GLfloat movementSpeed, GLfloat mouseSensitivity, 
				GLfloat scrollSensitivity, GLfloat fov) :
	Position(position), Up(up), Yaw(yaw), Pitch(pitch), 
	MovementSpeed(movementSpeed),MouseSensitivity(mouseSensitivity), 
	ScrollSensitivity(scrollSensitivity), Fov(fov)
{
	physicsEngine=new PhysicsEngine;
	VertVelocity=glm::vec3(0.0f);
	accelerUp=glm::vec3(0.0f);
	isJumping=false;
	UpdateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
				GLfloat upX, GLfloat upY, GLfloat upZ,
				GLfloat yaw, GLfloat pitch,
				GLfloat movementSpeed, GLfloat mouseSensitivity, 
				GLfloat scrollSensitivity, GLfloat fov) :
	Position(posX, posY, posZ), Up(upX, upY, upZ), Yaw(yaw), Pitch(pitch),
	MovementSpeed(movementSpeed), MouseSensitivity(mouseSensitivity), 
	ScrollSensitivity(scrollSensitivity) ,Fov(fov)
{
	physicsEngine=new PhysicsEngine;
	VertVelocity=glm::vec3(0.0f);
	accelerUp=glm::vec3(0.0f);
	isJumping=false;
	UpdateCameraVectors();
}

Camera::Camera():
	Position(glm::vec3(0.0f, 0.0f, 0.0f)), 
	Up(glm::vec3(0.0f, 1.0f, 0.0f)), 
	Yaw(__INIT_YAW), Pitch(__INIT_PITCH), 
	MovementSpeed(MoveSpeed),MouseSensitivity(__INIT_MOUSE_SENSITIVITY), 
	ScrollSensitivity(__INIT_SCROLL_SENSITIVITY) ,Fov(__INIT_FOV)
{
	physicsEngine=new PhysicsEngine;
	VertVelocity=glm::vec3(0.0f);
	accelerUp=glm::vec3(0.0f);
	isJumping=false;
	UpdateCameraVectors();
}
Camera::~Camera()
{
	delete physicsEngine;
}

void Camera::jumpAndUpdateVelocity() {
	VertVelocity = glm::vec3(0.f, JumpInitialSpeed, 0.f);
	accelerUp.y = 0.f;
	isJumping = true;
}


// Update the camera's position corresponding to the keyboard event.
// deltatime just to eliminate the performance among different PCs.
GLvoid Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = MovementSpeed * deltaTime;
	float dx=0,dz=0;
	if (direction == FORWARD)
		dz = velocity;
	if (direction == BACKWARD)
		dz = -velocity;
	if (direction == LEFT)
		dx = -velocity;
	if (direction == RIGHT)
		dx = velocity;
	if (direction == JUMP){
		if (!isJumping) {
			jumpAndUpdateVelocity();
		}
	}

	if(!dx || !dz){
		Position += dz * BodyFront + dx * BodyRight;
		TargetPos = Position + 10.0f * (dz * BodyFront + dx * BodyRight);
		
		physicsEngine->outCollisionTest(Position, TargetPos);
		physicsEngine->inCollisionTest(Position, TargetPos, HeroHeight);
	}
}

GLvoid Camera::LoopFunction(GLfloat deltaTime)
{
	physicsEngine->updateCameraVertMovement(Position,TargetPos,VertVelocity,accelerUp,isJumping,HeroHeight,deltaTime);
	if(Position.y - HeroHeight < 0.0f)
	{
		Position.y = HeroHeight;
		isJumping=false;
	}
}


// Update the camera's Euler angle corresponding to the mouse move event.
GLvoid Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw		+= xoffset;
	Pitch	+= yoffset;

	if (Pitch > __MAX_PITCH)
		Pitch = __MAX_PITCH;
	if (Pitch < __MIN_PITCH)
		Pitch = __MIN_PITCH;

	UpdateCameraVectors();
}

// Update the camera's zoom corresponding to the mouse scroll event.
GLvoid Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (Fov >= __MIN_FOV && Fov <= __MAX_FOV)
		Fov -= yoffset * ScrollSensitivity;
	if (Fov <= __MIN_FOV)
		Fov = __MIN_FOV;
	if (Fov >= __MAX_FOV)
		Fov = __MAX_FOV;
}

// initial or calculate the front vector after update the Euler Angle.
GLvoid Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	// hero head and eyes
	EyeFront	= glm::normalize(front);
	Right		= glm::normalize(glm::cross(EyeFront, glm::vec3(0.0f,1.0f,0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up			= glm::normalize(glm::cross(Right, EyeFront));
	// hero body
	BodyFront   = glm::normalize(glm::vec3(front.x,0.0f,front.z));
	BodyUp 		= glm::vec3(0.0f,1.0f,0.0f);
	BodyRight 	= glm::normalize(glm::cross(BodyFront,BodyUp));
}

// Get the camera's corresponding view Matrix
const glm::mat4 Camera::GetViewMatrix()
{
	// notice that: Front = CameraTarget - Position
	return glm::lookAt(Position, Position + EyeFront, Up);
}

// Get Camera's Attributes
const glm::vec3& Camera::GetPosition()
{
	return Position;
}

inline const glm::vec3& Camera::GetTargetPos()
{
	return TargetPos;
}

const glm::vec3& Camera::GetEyeFront()
{
	return EyeFront;
}

const glm::vec3& Camera::GetBodyFront()
{
	return BodyFront;
}

const glm::vec3& Camera::GetBodyRight()
{
	return BodyRight;
}

const glm::vec3& Camera::GetBodyUp()
{
	return BodyUp;
}

const glm::vec3& Camera::GetUp()
{
	return Up;
}

const glm::vec3& Camera::GetRight()
{
	return Right;
}

// Get Camera's Euler Angle
const GLfloat Camera::GetYaw()
{
	return Yaw;
}

const GLfloat Camera::GetPitch()
{
	return Pitch;
}

// Get Camera's options
const GLfloat Camera::GetMovementSpeed()
{
	return MovementSpeed;
}

const GLfloat Camera::GetMouseSensitivity()
{
	return MouseSensitivity;
}

const GLfloat Camera::GetScrollSensitivity()
{
	return ScrollSensitivity;
}

const GLfloat Camera::GetFov()
{
	return Fov;
}

// Set Physics Engine's Attributes
GLvoid Camera::SetOuterBound(const glm::vec4 &outBound)
{
	physicsEngine->setSceneOuterBoundary(outBound[0],outBound[1],outBound[2],outBound[3]);
}

GLvoid Camera::SetinnerBound(const glm::vec3 &b1,const glm::vec3 &b2)
{
	physicsEngine->setSceneInnerBoundary(b1[0],b1[1],b1[2],b2[0],b2[1],b2[2]);
}

// Set Camera's Attributes
GLvoid Camera::SetPosition(const glm::vec3 &position)
{
	Position = position;
}

inline GLvoid Camera::SetTargetPos(const glm::vec3 &position)
{
	TargetPos = position;
}

GLvoid Camera::SetBodyFront(const glm::vec3 &front)
{
	BodyFront = front;
}

GLvoid Camera::SetBodyRight(const glm::vec3 &right)
{
	BodyRight = right;
}

GLvoid Camera::SetBodyUp(const glm::vec3 &up)
{
	BodyUp = up;
}

GLvoid Camera::SetEyeFront(const glm::vec3 &front)
{
	EyeFront = front;
}

GLvoid Camera::SetUp(const glm::vec3 &up)
{
	Up = up;
}

GLvoid Camera::SetRight(const glm::vec3 &right)
{
	Right = right;
}

// Set Camera's Euler Angle
GLvoid Camera::SetYaw(const GLfloat &yaw)
{
	Yaw = yaw;
}

GLvoid Camera::SetPitch(const GLfloat &pitch)
{
	Pitch = pitch;
}

// Set Camera's options
GLvoid Camera::SetMovementSpeed(const GLfloat &movementSpeed)
{
	MovementSpeed = movementSpeed;
}

GLvoid Camera::SetMouseSensitivity(const GLfloat &mouseSensitivity)
{
	MouseSensitivity = mouseSensitivity;
}

GLvoid Camera::SetScrollSensitivity(const GLfloat &scrollSensitivity)
{
	ScrollSensitivity = scrollSensitivity;
}

GLvoid Camera::SetFov(const GLfloat &fov)
{
	Fov = fov;
}
