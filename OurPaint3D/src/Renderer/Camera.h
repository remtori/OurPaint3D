#pragma once

#include <glm/glm.hpp>

class Camera
{
public:	
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	float Yaw;
	float Pitch;	

	// Field of view
	float FOV;
	float aspectRatio;

	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f
	);
	~Camera();

	glm::mat4 GetViewMatrix() const;	
	glm::mat4 GetProjectionMatrix() const;
	void ReCalc();
};

