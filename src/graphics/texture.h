#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_TEXTURE_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_TEXTURE_H_

#include "texture_region.h"

#include <string>

namespace graphics {

class Texture {
 public:
  explicit Texture(std::string const &path);

  void Bind();
  static void Unbind();
  TextureRegion Region(float x, float y, float width, float height);

  int width() const { return width_; };
  int height() const { return height_; };

 private:
  unsigned int texture_;
  int width_;
  int height_;

  static unsigned int binded_;
};

}

#endif
