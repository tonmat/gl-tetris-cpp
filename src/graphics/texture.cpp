#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>

namespace graphics {

unsigned int Texture::binded_;

Texture::Texture(std::string const &path) {
  int channels;
  auto pixels = stbi_load(path.c_str(), &width_, &height_, &channels, STBI_rgb_alpha);
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(pixels);
}

void Texture::Bind() {
  if (binded_ == texture_)
    return;
  binded_ = texture_;
  glBindTexture(GL_TEXTURE_2D, binded_);

}

void Texture::Unbind() {
  if (binded_ == 0)
    return;
  binded_ = 0;
  glBindTexture(GL_TEXTURE_2D, binded_);
}

TextureRegion Texture::Region(float x, float y, float width, float height) {
  return {
      .x =  x,
      .y =  y,
      .width =  width,
      .height =  height,
      .s0 = x / this->width_,
      .t0 = y / this->height_,
      .s1 = (x + width) / this->width_,
      .t1 = (y + height) / this->height_
  };
}

}