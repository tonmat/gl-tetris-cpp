#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_GRAPHICS_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_GRAPHICS_H_

#include "config.h"
#include <string>

struct GLFWwindow;

namespace graphics {

std::string Read(const std::string &path);

void Run(Config const &config);

GLFWwindow *Window();

}

#endif
