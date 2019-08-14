#include "shape.h"

#include <algorithm>
#include <random>
#include <chrono>

static std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
static std::uniform_int_distribution<std::mt19937::result_type> dis(0, 6);
static game::Shape kShapes[]{
    {4, 0b0000111100000000},
    {3, 0b010111000},
    {3, 0b100111000},
    {3, 0b001111000},
    {3, 0b110011000},
    {3, 0b011110000},
    {2, 0b1111},
};

namespace game {

Shape::Shape(unsigned char size, unsigned short mask) : size_(size), mask_(mask) {
  CalculateCenter();
}

void Shape::Set(Shape &shape) {
  size_ = shape.size_;
  mask_ = shape.mask_;
  CalculateCenter();
}

bool Shape::HasCell(unsigned char x, unsigned char y) const {
  return (mask_ >> (x + y * size_) & 1u) == 1u;
}

void Shape::Clear() {
  size_ = 0;
  mask_ = 0;
  center_x_ = 0;
  center_y_ = 0;
}

void Shape::Random() {
  Set(kShapes[dis(gen)]);
}

void Shape::Rotate(char direction) {
  if (direction < 0)
    RotateCCW();
  else if (direction > 0)
    RotateCW();
}

void Shape::RotateCW() {
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

void Shape::RotateCCW() {
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
      max_x = std::max(static_cast<unsigned char>(x + 1u), max_x);
      max_y = std::max(static_cast<unsigned char>(y + 1u), max_y);
    }
  center_x_ = static_cast<float>(min_x + max_x) / 2.0f;
  center_y_ = static_cast<float>(min_y + max_y) / 2.0f;
}

}
