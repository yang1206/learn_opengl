#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

class Camera {
public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH);

  // 获取观察矩阵
  glm::mat4 GetViewMatrix() const;
  // 获取投影矩阵
  glm::mat4 GetProjectionMatrix() const;
  // 处理输入
  void ProcessKeyboard(Camera_Movement direction, float deltaTime);
  void ProcessMouseMovement(float xpos, float ypos);
  void ProcessMouseScroll(float yoffset);

  // 设置投影参数
  void SetAspectRatio(float aspect);
  void SetNearPlane(float nearPlane);
  void SetFarPlane(float farPlane);
  void SetFov(float fov);

  // 鼠标控制
  void SetMouseControl(bool enabled);
  bool IsMouseEnabled() const;
  void ResetMouseState(float xpos, float ypos);

  // 获取相机参数
  float GetFov() const;
  float GetNearPlane() const;
  float GetFarPlane() const;
  glm::vec3 GetPosition() const;
  glm::vec3 GetFront() const;
  glm::vec3 GetUp() const;
  glm::vec3 GetRight() const;

  // 设置相机参数
  void SetPosition(const glm::vec3 &position);
  void SetMovementSpeed(float speed);
  void SetMouseSensitivity(float sensitivity);

  struct CameraState {
    glm::vec3 Position;
    float Yaw;
    float Pitch;
    float Zoom;
  };

  CameraState GetState() const { return {Position, Yaw, Pitch, Zoom}; }

  void SetState(const CameraState &state) {
    Position = state.Position;
    Yaw = state.Yaw;
    Pitch = state.Pitch;
    Zoom = state.Zoom;
    updateCameraVectors();
  }

  void Reset() {
    Position = glm::vec3(0.0f, 0.0f, 3.0f);
    Yaw = YAW;
    Pitch = PITCH;
    Zoom = ZOOM;
    updateCameraVectors();
  }

private:
  void updateCameraVectors();

  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  // euler Angles
  float Yaw;
  float Pitch;

  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // projection parameters
  float AspectRatio;
  float NearPlane;
  float FarPlane;

  bool mouseEnabled;
  bool firstMouse;
  float lastX;
  float lastY;

  float smoothFactor = 0.1f;
  glm::vec3 targetPosition;
  glm::vec3 currentVelocity = glm::vec3(0.0f);
};
