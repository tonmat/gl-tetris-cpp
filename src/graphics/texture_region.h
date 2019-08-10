#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_TEXTURE_REGION_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_TEXTURE_REGION_H_

namespace graphics {

struct TextureRegion {
  const float x;
  const float y;
  const float width;
  const float height;
  const float s0;
  const float t0;
  const float s1;
  const float t1;
};

}

#endif
