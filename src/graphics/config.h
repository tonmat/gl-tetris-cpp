#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_CONFIG_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_CONFIG_H_

#include "color.h"
#include <string>

namespace graphics {

using Callback = void (*)();
using UpdateCallback = void (*)(float delta);
using RenderCallback = void (*)(double time);

struct Config {
  std::string title;
  int width;
  int height;
  float update_rate;
  Color clear_color;
  Callback on_initialize;
  UpdateCallback on_update;
  RenderCallback on_render;
};

}

#endif
