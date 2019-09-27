#include "Camera.h"

const float MOVEMENT_SPEED = 4.0f;

Camera::Camera() :
	viewDirection(-0.011437, -0.186789, -0.982333),
	UP(0.0f, 1.0f, 0.0f),
	position(0.013748, 0.173624, 1.947336)
{
	yaw = 0.0f;
	pitch = 0.0f;
}

void Camera::mouseUpdate(const glm::vec2 &newMousePosition) {
	
	
	
	/*glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 100.0f) {
		oldMousePosition = newMousePosition;
		return;
	}
	const float ROTATIONAL_SPEED = 0.005f;
	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP) *
		glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, toRotateAround);
	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;*/
	yaw += newMousePosition.x;
	pitch += newMousePosition.y;
	glm::mat4 Rotation = glm::yawPitchRoll(glm::radians(yaw), glm::radians(pitch), 0.0f);
	viewDirection = glm::vec3(Rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	UP = glm::vec3(Rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	//printf("position (%f,%f,%f) viewDiw (%f,%f,%f)\n", position[0], position[1], position[2], viewDirection[0], viewDirection[1], viewDirection[2]);
	return glm::lookAt(position, position + viewDirection, UP);
}

void Camera::moveForward(float time) {
	float speed = MOVEMENT_SPEED * time;
	position += speed * viewDirection;
}

void Camera::moveBackward(float time) {
	float speed = MOVEMENT_SPEED * time;
	position -= speed * viewDirection;
}

void Camera::moveLeft(float time) {
	float speed = MOVEMENT_SPEED * time;
	glm::vec3 moveDirection = glm::cross(viewDirection, UP);
	position += -speed * moveDirection;
}

void Camera::moveRight(float time) {
	float speed = MOVEMENT_SPEED * time;
	glm::vec3 moveDirection = glm::cross(viewDirection, UP);
	position += speed * moveDirection;
}

void Camera::moveUp(float time) {
	float speed = MOVEMENT_SPEED * time;
	position += speed * UP;
}

void Camera::moveDown(float time) {
	float speed = MOVEMENT_SPEED * time;
	position -= speed * UP;
}