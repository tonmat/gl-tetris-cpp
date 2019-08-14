#include "inputs.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace inputs {

const int kP = GLFW_KEY_P;
const int kR = GLFW_KEY_R;
const int kLeft = GLFW_KEY_LEFT;
const int kRight = GLFW_KEY_RIGHT;
const int kDown = GLFW_KEY_DOWN;
const int kUp = GLFW_KEY_UP;
const int kLeftControl = GLFW_KEY_LEFT_CONTROL;
const int kSpace = GLFW_KEY_SPACE;

Inputs::Inputs(GLFWwindow *window) : window_(window) {
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

void Inputs::Update(float delta) {
  time_ += delta;
}

bool Inputs::isDown(int code, float repeat1, float repeatN) {
  if (code < 0 || code > 512)
    return false;

  switch (glfwGetKey(window_, code)) {
    case GLFW_PRESS:
      if (state_[code] == -1) {
        state_[code] = time_ + repeat1;
        return true;
      } else if (time_ >= state_[code]) {
        state_[code] = time_ + repeatN;
        return true;
      }
      break;
    default:state_[code] = -1;
      break;
  }

  return false;
}

bool Inputs::isJustPressed(int code) {
  if (code < 0 || code > 512)
    return false;

  switch (glfwGetKey(window_, code)) {
    case GLFW_PRESS:
      if (state_[code] == -1) {
        state_[code] = time_;
        return true;
      }
      break;
    default:state_[code] = -1;
      break;
  }

  return false;
}

}
