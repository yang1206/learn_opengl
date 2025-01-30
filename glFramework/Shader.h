#pragma once
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include <core.h>
#include <string>
#include <unordered_map>

class Shader {
public:
  unsigned int ID;

  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();
  void use();
  void unuse() const;

  // 禁用拷贝构造和赋值操作
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  // 获取着色器程序ID
  unsigned int getID() const { return ID; }

  // uniform 设置函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

  // 向量设置函数
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z,
               float w) const;

  // 矩阵设置函数
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
  void checkCompileErrors(unsigned int shader, std::string type);

  // 缓存uniform位置
  mutable std::unordered_map<std::string, int> uniformLocationCache;
  int getUniformLocation(const std::string &name) const;
};
