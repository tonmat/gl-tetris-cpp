#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_LABEL_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_LABEL_H_

#include "color.h"
#include "align.h"
#include <string>

namespace graphics {

const size_t kLabelTextMaxSize = 255;

struct Label {
  char text[kLabelTextMaxSize];
  float x;
  float y;
  HAlign h_align = HAlign::kLeft;
  float h_spacing = .75f;
  float v_spacing = .75f;
  float size = 1.0f;
  Color color;
  float alpha = 1.0f;
};

}

#endif
