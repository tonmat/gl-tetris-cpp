#include "board.h"

namespace game {

Board::Board(unsigned char width, unsigned char height) : width_(width), height_(height) {
  rows_ = new Row[height];
}

Board::~Board() {
  delete[] rows_;
}

bool Board::IsFullRow(unsigned char y) {
  return rows_[y].mask_ == (1u << width_) - 1;
}

void Board::RemoveRow(unsigned char y) {
  for (unsigned char i = y; i < height_ - 1; i++)
    rows_[i] = rows_[i + 1];
  rows_[height_ - 1].mask_ = 0u;
}

Row const &Board::operator[](unsigned char y) const {
  return rows_[y];
}

}
