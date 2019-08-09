#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int kWidth = 600;
const int kHeight = 800;

int main() {
  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Could not initialize GLFW" << std::endl;
    return 1;
  }

  auto monitor = glfwGetPrimaryMonitor();
  if (monitor == nullptr) {
    std::cerr << "Could not get primary monitor" << std::endl;
    glfwTerminate();
    return 2;
  }

  auto videoMode = glfwGetVideoMode(monitor);
  if (videoMode == nullptr) {
    std::cerr << "Could not get video mode" << std::endl;
    glfwTerminate();
    return 3;
  }

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  auto window = glfwCreateWindow(kWidth, kHeight, "gl-tetris-cpp", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Could not create window" << std::endl;
    glfwTerminate();
    return 4;
  }

  auto xpos = (videoMode->width - kWidth) / 2;
  auto ypos = (videoMode->height - kHeight) / 2;
  glfwSetWindowPos(window, xpos, ypos);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glewInit();

  std::cout << "GLFW " << glfwGetVersionString() << std::endl;
  std::cout << "GLEW " << glewGetString(GLEW_VERSION) << std::endl;
  std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << glGetString(GL_VENDOR) << std::endl;

  glfwShowWindow(window);

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwHideWindow(window);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}