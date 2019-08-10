#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_SPRITE_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_SPRITE_H_

#include "color.h"
#include "texture_region.h"

namespace graphics {

struct Sprite {
  float x;
  float y;
  float width = 1.0f;
  float height = 1.0f;
  TextureRegion texture_region;
  Color color;
  float alpha = 1.0f;
};

}

#endif
