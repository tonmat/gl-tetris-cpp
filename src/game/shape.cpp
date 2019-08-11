#include "shape.h"

#include <algorithm>

namespace game {

Shape::Shape(unsigned char size, unsigned short mask) : size_(size), mask_(mask) {
  CalculateCenter();
}

void Shape::Set(Shape &shape) {
  size_ = shape.size_;
  mask_ = shape.mask_;
  CalculateCenter();
}

bool Shape::HasCell(unsigned char x, unsigned char y) {
  return mask_ >> (x + y * size_) == 1u;
}

void Shape::RotateCW() {
  unsigned short new_mask = 0;
  for (unsigned char y = 0; y < size_; y++)
    for (unsigned char x = 0; x < size_; x++) {
      if (!HasCell(x, y))
        continue;
      unsigned char nx = size_ - 1 - y;
      unsigned char ny = x;
      new_mask |= 1u << (nx + ny * size_);
    }
  mask_ = new_mask;
  CalculateCenter();
}

void Shape::RotateCCW() {
  unsigned short new_mask = 0;
  for (unsigned char y = 0; y < size_; y++)
    for (unsigned char x = 0; x < size_; x++) {
      if (!HasCell(x, y))
        continue;
      unsigned char nx = y;
      unsigned char ny = size_ - 1 - x;
      new_mask |= 1u << (nx + ny * size_);
    }
  mask_ = new_mask;
  CalculateCenter();
}
void Shape::CalculateCenter() {
  unsigned char min_x = size_ - 1;
  unsigned char min_y = size_ - 1;
  unsigned char max_x = 0;
  unsigned char max_y = 0;
  for (unsigned char y = 0; y < size_; y++)
    for (unsigned char x = 0; x < size_; x++) {
      if (!HasCell(x, y))
        continue;
      min_x = std::min(x, min_x);
      min_y = std::min(y, min_y);
      max_x = std::max(x, max_x);
      max_y = std::max(y, max_y);
    }
  center_x_ = static_cast<float>(min_x + max_x) / 2;
  center_y_ = static_cast<float>(min_y + max_y) / 2;
}

}
