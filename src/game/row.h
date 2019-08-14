#ifndef GL_TETRIS_CPP_SRC_GAME_ROW_H_
#define GL_TETRIS_CPP_SRC_GAME_ROW_H_

namespace game {

class Row {
 public:
  Row();
  virtual ~Row() = default;

  [[nodiscard]] bool HasCell(unsigned char x) const;
  void SetCell(unsigned char x);

 private:
  unsigned short mask_;

  friend class Board;
};

}

#endif
