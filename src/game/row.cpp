#include "row.h"

namespace game {

Row::Row() {
  mask_ = 0u;
}

bool Row::HasCell(unsigned char x) {
  return (mask_ >> x) == 1u;
}

}
