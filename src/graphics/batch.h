#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_BATCH_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_BATCH_H_

#include "vertex.h"
#include "sprite.h"
#include "label.h"

namespace graphics {

class Batch {
 public:
  explicit Batch(unsigned int capacity);

  void Begin();
  void End();
  void Draw(Sprite const &sprite);
  void Draw(Label const &label);

 private:
  void AddIndices();
  void AddSpriteVertex(Sprite const &sprite, float w, float h);
  void AddLabelVertex(Label const &label, float x, float y, float w, float h, float s, float t);
  void DrawChar(Label const &label, char c, float x, float y);
  float GetLabelWidth(Label const &label);

  unsigned int capacity_;
  unsigned int count_;

  unsigned int vertices_capacity_;
  unsigned int vertices_size_;
  Vertex *vertices_;

  unsigned int indices_capacity_;
  unsigned int indices_size_;
  unsigned short *indices_;

  bool drawing_;

  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ibo_;

};

}

#endif
