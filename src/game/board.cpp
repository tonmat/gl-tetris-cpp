#include <iostream>
#include "board.h"

namespace game {

Board::Board(unsigned char width, unsigned char height) : width_(width), height_(height) {
  rows_ = new Row[height];
}

Board::~Board() {
  delete[] rows_;
}

bool Board::CheckPlayer(Player const &player, bool ignoreRow) const {
  if (player.size() <= 0)
    return false;
  for (unsigned char y = 0; y < player.size(); y++) {
    auto by = player.y + y;
    auto &row = rows_[by];
    for (unsigned char x = 0; x < player.size(); x++) {
      if (!player.HasCell(x, y))
        continue;
      auto bx = player.x + x;
      if (IsOutOfBounds(bx, by))
        return false;
      if (!ignoreRow && row.HasCell(bx))
        return false;
    }
  }
  return true;
}

unsigned char Board::PlacePlayer(Player const &player) {
  unsigned char rows = 0;
  for (unsigned char y = 0; y < player.size(); y++) {
    auto by = player.y + y;
    auto &row = rows_[by];
    for (unsigned char x = 0; x < player.size(); x++) {
      if (!player.HasCell(x, y))
        continue;
      auto bx = player.x + x;
      if (IsOutOfBounds(bx, by))
        continue;
      row.SetCell(bx);
    }
    if (!IsOutOfBounds(0, by) && IsFullRow(by))
      rows++;
  }
  return rows;
}

bool Board::IsOutOfBounds(char x, char y) const {
  return x < 0 || x >= width_ || y < 0 || y >= height_;
}

bool Board::IsFullRow(unsigned char y) const {
  return rows_[y].mask_ == (1u << width_) - 1;
}

void Board::RemoveRow(unsigned char y) {
  for (unsigned char i = y; i < height_ - 1; i++)
    rows_[i] = rows_[i + 1];
  rows_[height_ - 1].mask_ = 0u;
}

void Board::Clear() {
  for (unsigned char i = 0; i < height_; i++)
    rows_[i].mask_ = 0u;
}

Row const &Board::operator[](unsigned char y) const {
  return rows_[y];
}

}
