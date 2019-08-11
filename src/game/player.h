#ifndef GL_TETRIS_CPP_SRC_GAME_PLAYER_H_
#define GL_TETRIS_CPP_SRC_GAME_PLAYER_H_

#include "shape.h"

namespace game {

class Player : public Shape {
 public:
  Player();
  ~Player() override = default;

  void Set(Shape &shape) override;

  char x;
  char y;
};

}

#endif
