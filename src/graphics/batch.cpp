#include "batch.h"

#include <iostream>
#include <GL/glew.h>

#define OFFSET(offset) reinterpret_cast<void *>(offset)

namespace graphics {

Batch::Batch(unsigned int capacity) {
  capacity_ = capacity;
  count_ = 0;

  vertices_capacity_ = capacity * 4;
  vertices_size_ = 0;
  vertices_ = new Vertex[vertices_capacity_];

  indices_capacity_ = capacity * 6;
  indices_size_ = 0;
  indices_ = new unsigned short[indices_capacity_];

  drawing_ = false;

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ibo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_capacity_ * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(Vertex), OFFSET(0));
  glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), OFFSET(4 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_capacity_ * sizeof(unsigned short), nullptr, GL_STREAM_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void Batch::Begin() {
  if (drawing_)
    return;
  drawing_ = true;

  count_ = 0;
  vertices_size_ = 0;
  indices_size_ = 0;
}

void Batch::End() {
  if (!drawing_)
    return;
  drawing_ = false;

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size_ * sizeof(Vertex), vertices_);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices_size_ * sizeof(unsigned short), indices_);
  glDrawElements(GL_TRIANGLES, indices_size_, GL_UNSIGNED_SHORT, nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Batch::Draw(Sprite const &sprite) {
  auto new_count = count_ + 1;
  if (new_count > capacity_) {
    std::cerr << "sprite overflow " << new_count << " > " << capacity_ << std::endl;
    return;
  }

  AddIndices();
  AddSpriteVertex(sprite, 0, 1);
  AddSpriteVertex(sprite, 0, 0);
  AddSpriteVertex(sprite, 1, 1);
  AddSpriteVertex(sprite, 1, 0);
  count_ = new_count;
}

void Batch::AddIndices() {
  auto new_size = indices_size_ + 6;
  if (indices_size_ > indices_capacity_) {
    std::cerr << "index array buffer overflow " << new_size << " > " << indices_capacity_ << std::endl;
    return;
  }

  indices_[indices_size_++] = vertices_size_;
  indices_[indices_size_++] = vertices_size_ + 1;
  indices_[indices_size_++] = vertices_size_ + 2;
  indices_[indices_size_++] = vertices_size_ + 2;
  indices_[indices_size_++] = vertices_size_ + 1;
  indices_[indices_size_++] = vertices_size_ + 3;
}

void Batch::AddSpriteVertex(Sprite const &sprite, float w, float h) {
  auto new_size = vertices_size_ + 1;
  if (vertices_size_ > vertices_capacity_) {
    std::cerr << "vertex array buffer overflow " << new_size << " > " << vertices_capacity_ << std::endl;
    return;
  }

  auto &vertex = vertices_[vertices_size_++];
  vertex.x = sprite.x + w * sprite.width;
  vertex.y = sprite.y + h * sprite.height;
  vertex.s = w * sprite.texture_region.s1 + (1 - w) * sprite.texture_region.s0;
  vertex.t = h * sprite.texture_region.t1 + (1 - h) * sprite.texture_region.t0;
  vertex.r = sprite.color.red;
  vertex.g = sprite.color.green;
  vertex.b = sprite.color.blue;
  vertex.a = sprite.alpha;
}
}
