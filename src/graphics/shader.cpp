#include "shader.h"
#include "GL/glew.h"
#include "graphics.h"

#include <iostream>
#include <filesystem>

namespace graphics {

unsigned int Shader::binded_;

Shader::Shader(std::vector<std::string> paths) {
  std::vector<int> shaders;
  for (auto &path : paths)
    shaders.push_back(CreateShader(path));

  program_ = glCreateProgram();
  for (auto &shader : shaders)
    glAttachShader(program_, shader);
  glLinkProgram(program_);
  for (auto &shader : shaders)
    glDeleteShader(shader);

  GLint status = 0;
  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    GLint length = 0;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
    GLchar info_log[length];
    glGetProgramInfoLog(program_, length, &length, info_log);
    std::cerr << "Error linking program " << std::endl << info_log << std::endl;
    glDeleteProgram(program_);
  }
}

void Shader::Bind() {
  if (binded_ == program_)
    return;
  binded_ = program_;
  glUseProgram(binded_);
}

void Shader::Unbind() {
  if (binded_ == 0)
    return;
  binded_ = 0;
  glUseProgram(binded_);
}

void Shader::SetUniform1i(std::string const &name, int x) {
  glUniform1i(GetUniform(name), x);
}

void Shader::SetUniformMat4(const std::string &name, const glm::mat4 &mat4) {
  glUniformMatrix4fv(GetUniform(name), 1, GL_TRUE, &mat4[0][0]);
}

unsigned int Shader::CreateShader(std::string const &path) {
  auto extension = std::filesystem::path(path).extension();
  GLenum type;
  if (extension == ".vert")
    type = GL_VERTEX_SHADER;
  else if (extension == ".frag")
    type = GL_FRAGMENT_SHADER;
  else {
    std::cerr << "Invalid shader type: " << extension << std::endl;
    return 0;
  }

  auto source = Read(path);
  auto shader = glCreateShader(type);
  GLchar const *c_str = source.c_str();
  GLint size = source.size();
  glShaderSource(shader, 1, &c_str, &size);

  glCompileShader(shader);
  GLint status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    GLchar info_log[length];
    glGetShaderInfoLog(shader, length, &length, info_log);
    std::cerr << "Error compiling shader " << std::endl << info_log << std::endl;
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

int Shader::GetUniform(std::string const &name) {
  auto key = uniform_locations_.find(name);
  if (key == uniform_locations_.end()) {
    auto location = glGetUniformLocation(program_, name.c_str());
    if (location == -1)
      std::cerr << "Uniform not found: " << name << std::endl;
    uniform_locations_.insert(std::make_pair(name, location));
    return location;
  }
  return (*key).second;
}

}