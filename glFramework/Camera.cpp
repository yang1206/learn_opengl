#include "Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM), AspectRatio(16.0f / 9.0f),
      NearPlane(NEAR_PLANE), FarPlane(FAR_PLANE), mouseEnabled(false),
      firstMouse(true), lastX(800.0f / 2.0f), lastY(600.0f / 2.0f) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
  return glm::perspective(glm::radians(Zoom), AspectRatio, NearPlane, FarPlane);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
  float velocity = MovementSpeed * deltaTime;
  glm::vec3 targetOffset(0.0f);

  if (direction == FORWARD)
    targetOffset += Front * velocity;
  if (direction == BACKWARD)
    targetOffset -= Front * velocity;
  if (direction == LEFT)
    targetOffset -= Right * velocity;
  if (direction == RIGHT)
    targetOffset += Right * velocity;

  targetPosition = Position + targetOffset;
  Position = glm::mix(Position, targetPosition, smoothFactor);
  Position.y = 0.0f;
}

void Camera::ProcessMouseMovement(float xpos, float ypos) {
  if (!mouseEnabled)
    return;

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
    return;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  if (Pitch > 89.0f)
    Pitch = 89.0f;
  if (Pitch < -89.0f)
    Pitch = -89.0f;

  updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
  Zoom -= yoffset;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 90.0f)
    Zoom = 90.0f;
}

void Camera::SetAspectRatio(float aspect) {
  AspectRatio = std::max(0.1f, aspect);
}

void Camera::SetNearPlane(float nearPlane) { NearPlane = nearPlane; }

void Camera::SetFarPlane(float farPlane) { FarPlane = farPlane; }

void Camera::SetFov(float fov) {
  Zoom = fov;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 90.0f)
    Zoom = 90.0f;
}

void Camera::SetMouseControl(bool enabled) {
  mouseEnabled = enabled;
  if (enabled) {
    firstMouse = true;
  }
}

bool Camera::IsMouseEnabled() const { return mouseEnabled; }

void Camera::ResetMouseState(float xpos, float ypos) {
  lastX = xpos;
  lastY = ypos;
  firstMouse = false;
}

float Camera::GetFov() const { return Zoom; }

float Camera::GetNearPlane() const { return NearPlane; }

float Camera::GetFarPlane() const { return FarPlane; }

glm::vec3 Camera::GetPosition() const { return Position; }

glm::vec3 Camera::GetFront() const { return Front; }

glm::vec3 Camera::GetUp() const { return Up; }

glm::vec3 Camera::GetRight() const { return Right; }

void Camera::SetPosition(const glm::vec3 &position) { Position = position; }

void Camera::SetMovementSpeed(float speed) {
  MovementSpeed = std::max(0.1f, speed);
}

void Camera::SetMouseSensitivity(float sensitivity) {
  MouseSensitivity = std::clamp(sensitivity, 0.01f, 1.0f);
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}