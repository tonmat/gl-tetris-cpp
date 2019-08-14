#include "row.h"

namespace game {

Row::Row() {
  mask_ = 0u;
}

bool Row::HasCell(unsigned char x) const {
  return (mask_ >> x & 1u) == 1u;
}

void Row::SetCell(unsigned char x) {
  mask_ |= 1u << x;
}

}
