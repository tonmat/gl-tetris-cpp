#ifndef GL_TETRIS_CPP_SRC_GAME_BOARD_H_
#define GL_TETRIS_CPP_SRC_GAME_BOARD_H_

#include "row.h"
#include "player.h"

namespace game {

class Board {
 public:
  Board(unsigned char width, unsigned char height);
  virtual ~Board();

  [[nodiscard]] bool CheckPlayer(Player const &player, bool ignoreRow = false) const;
  unsigned char PlacePlayer(Player const &player);
  [[nodiscard]] bool IsOutOfBounds(char x, char y) const;
  [[nodiscard]] bool IsFullRow(unsigned char y) const;
  void RemoveRow(unsigned char y);
  void Clear();

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
