#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/batch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glClearColor(1, 1, 1, 1);
  glClearColor(0, .025f, 0, 1);
  glActiveTexture(GL_TEXTURE0);
  glViewport(0, 0, kWidth, kHeight);

  auto projection = glm::ortho(0.0f, 15.0f, 0.0f, 20.0f);

  auto shader = new graphics::Shader({"shaders/shader.vert", "shaders/shader.frag"});
  shader->Bind();
  shader->SetUniform1i("u_material", 0);
  shader->SetUniformMat4("u_projection", projection);

  auto texture = new graphics::Texture("textures/spritesheet.png");
  texture->Bind();
  auto region = texture->Region(0, 64, 8, 8);

  graphics::Sprite sprite = {
      .texture_region = region
  };

  auto batch = new graphics::Batch(2048);

  glfwShowWindow(window);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    batch->Begin();
    batch->Draw(sprite);
    batch->End();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwHideWindow(window);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}