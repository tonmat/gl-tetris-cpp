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

void Batch::Draw(Label const &label) {
  float w;
  switch (label.h_align) {
    default:
      w = 0.0f;
      break;
    case HAlign::kCenter:
      w = GetLabelWidth(label) / 2.0f;
      break;
    case HAlign::kRight:
      w = GetLabelWidth(label);
      break;
  }
  auto x = 0.0f;
  auto y = 0.0f;
  for (auto i = 0; i < kLabelTextMaxSize; i++) {
    auto c = label.text[i];
    if (c == '\0')
      break;
    if (c == '\n') {
      x = 0;
      y -= label.size * label.v_spacing;
      continue;
    }
    DrawChar(label, c, x - w, y);
    x += label.size * label.h_spacing;
  }
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

void Batch::AddLabelVertex(Label const &label, float x, float y, float w, float h, float s, float t) {
  auto new_size = vertices_size_ + 1;
  if (vertices_size_ > vertices_capacity_) {
    std::cerr << "vertex array buffer overflow " << new_size << " > " << vertices_capacity_ << std::endl;
    return;
  }

  auto &vertex = vertices_[vertices_size_++];
  vertex.x = label.x + x + w * label.size;
  vertex.y = label.y + y + h * label.size;
  vertex.s = s;
  vertex.t = t;
  vertex.r = label.color.red;
  vertex.g = label.color.green;
  vertex.b = label.color.blue;
  vertex.a = label.alpha;
}

void Batch::DrawChar(Label const &label, char c, float x, float y) {
  auto index = c - 32;
  auto s = index % 16;
  auto t = index / 16;
  auto s0 = s / 16.0f;
  auto t0 = t / 16.0f;
  auto s1 = (s + 1.0f) / 16.0f;
  auto t1 = (t + 1.0f) / 16.0f;
  AddIndices();
  AddLabelVertex(label, x, y, 0, 1, s0, t0);
  AddLabelVertex(label, x, y, 0, 0, s0, t1);
  AddLabelVertex(label, x, y, 1, 1, s1, t0);
  AddLabelVertex(label, x, y, 1, 0, s1, t1);
}

float Batch::GetLabelWidth(Label const &label) {
  auto w = 0;
  auto x = 0;
  for (auto i = 0; i < kLabelTextMaxSize; i++) {
    auto c = label.text[i];
    if (c == '\0')
      break;
    if (c == '\n') {
      w = std::max(w, x);
      x = 0;
      continue;
    }
    x++;
  }
  w = std::max(w, x);
  return label.size * (label.h_spacing * (w - 1) + 1);
}

}
