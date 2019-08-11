#include "player.h"

#include <typeinfo>

namespace game {

Player::Player() : Shape(0, 0) {
  x = 0;
  y = 0;
}

void Player::Set(Shape &shape) {
  Shape::Set(shape);
  try {
    auto player = dynamic_cast<Player &>(shape);
    x = player.x;
    y = player.y;
  } catch (std::bad_cast &) {
  }
}

}