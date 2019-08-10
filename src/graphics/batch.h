#ifndef GL_TETRIS_CPP_SRC_GRAPHICS_BATCH_H_
#define GL_TETRIS_CPP_SRC_GRAPHICS_BATCH_H_

#include "vertex.h"
#include "sprite.h"

namespace graphics {

class Batch {
 public:
  explicit Batch(unsigned int capacity);

  void Begin();
  void End();
  void Draw(Sprite const &sprite);

 private:
  void AddIndices();
  void AddSpriteVertex(Sprite const &sprite, float w, float h);

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
