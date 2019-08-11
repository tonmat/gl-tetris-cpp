#ifndef GL_TETRIS_CPP_SRC_GAME_BOARD_H_
#define GL_TETRIS_CPP_SRC_GAME_BOARD_H_

#include "row.h"

namespace game {

class Board {
 public:
  Board(unsigned char width, unsigned char height);
  virtual ~Board();

  bool IsFullRow(unsigned char y);
  void RemoveRow(unsigned char y);

  [[nodiscard]] unsigned char width() const { return width_; }
  [[nodiscard]] unsigned char height() const { return height_; }

  Row const &operator[](unsigned char y) const;

 private:
  unsigned char width_;
  unsigned char height_;
  Row *rows_;
};

}

#endif
