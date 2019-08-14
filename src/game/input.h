#ifndef GL_TETRIS_CPP_SRC_GAME_INPUT_H_
#define GL_TETRIS_CPP_SRC_GAME_INPUT_H_

namespace game {

struct Input {
  bool rotate_ccw;
  bool rotate_cw;
  bool move_left;
  bool move_right;
  bool drop_soft;
  bool drop_hard;
  bool hold;
};

}

#endif
