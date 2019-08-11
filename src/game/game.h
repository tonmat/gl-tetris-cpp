#ifndef GL_TETRIS_CPP_SRC_GAME_GAME_H_
#define GL_TETRIS_CPP_SRC_GAME_GAME_H_

#include "board.h"
#include "player.h"

namespace game {

class Game {
 public:
  Game(unsigned char boardWidth, unsigned char boardHeight);
  virtual ~Game();

  void Update(float delta);

  [[nodiscard]] float time() const { return time_; }
  [[nodiscard]]  Board const &board() const { return *board_; }
  [[nodiscard]]  Player const &player() const { return *player_; }
  [[nodiscard]]  Shape const &next() const { return *next_; }

 private:
  float time_;
  Board *board_;
  Player *player_;
  Shape *next_;
};

}

#endif
