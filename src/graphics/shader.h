#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_SHADER_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_SHADER_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/mat4x4.hpp>

namespace graphics {

class Shader {
 public:
  explicit Shader(std::vector<std::string> paths);

  void Bind();
  static void Unbind();
  void SetUniform1i(std::string const &name, int x);
  void SetUniformMat4(std::string const &name, glm::mat4 const &mat4);

 private:
  unsigned int CreateShader(std::string const &path);
  int GetUniform(std::string const &name);

  unsigned int program_;
  std::unordered_map<std::string, int> uniform_locations_;
  static unsigned int binded_;
};

}

#endif
