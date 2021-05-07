#pragma once

#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
// enum default works like: FORWARD = 0, BACKWARD = 1, ... then I can use the Camera_Movement type and I can only asign values 0 to 3 to it.

// Default camera values
const float YAW        = -90.0f;
const float PITCH      =  0.0f;
const float SPEED      =  20.0f;
const float SENSITIVTY =  0.03f;
const float ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
	//Default settings for reset function
	glm::vec3 InitialPosition;
	glm::vec3 InitialFront;
	glm::vec3 InitialUp;
	glm::vec3 InitialRight;
	glm::vec3 InitialWorldUp;
	float InitialYaw;
	float InitialPitch;
	float InitialZoom;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
	void ProcessMouseScroll(float yoffset);

	void resetCamera();

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
	
	// TODO: implement
	void setInitialCameraValues();


};

