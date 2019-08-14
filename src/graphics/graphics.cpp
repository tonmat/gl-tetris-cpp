#include "graphics.h"

#include <memory>
#include <filesystem>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace graphics {

static GLFWwindow *window;
static double ratio;

std::string Read(const std::string &path) {
  auto close_file = [](FILE *f) { fclose(f); };

  auto holder = std::unique_ptr<FILE, decltype(close_file)>(fopen(path.c_str(), "rb"), close_file);
  if (!holder)
    return "";
  FILE *f = holder.get();
  auto size = std::filesystem::file_size(path);
  std::string res;
  res.resize(size);
  fread(const_cast<char *>(res.data()), 1, size, f);
  return res;
}

void Resize(GLFWwindow *window, int width, int height) {
  std::cout << "Surface resized to " << width << " x " << height << std::endl;

  auto r = (double) width / height;
  auto w = width;
  auto h = height;
  if (r > ratio)
    w = height * ratio;
  else
    h = width / ratio;
  auto x = (width - w) / 2;
  auto y = (height - h) / 2;

  glViewport(x, y, w, h);
  std::cout << "Viewport set to " << w << " x " << h << std::endl;
}

void Run(Config const &config) {
  auto title = config.title.c_str();
  auto width = config.width;
  auto height = config.height;
  auto update_rate = config.update_rate;
  auto clear_color = config.clear_color;
  auto on_initialize = config.on_initialize;
  auto on_terminate = config.on_terminate;
  auto on_update = config.on_update;
  auto on_render = config.on_render;
  ratio = (double) width / height;

  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Could not initialize GLFW" << std::endl;
    return;
  }

  auto monitor = glfwGetPrimaryMonitor();
  if (monitor == nullptr) {
    std::cerr << "Could not get primary monitor" << std::endl;
    glfwTerminate();
    return;
  }

  auto videoMode = glfwGetVideoMode(monitor);
  if (videoMode == nullptr) {
    std::cerr << "Could not get video mode" << std::endl;
    glfwTerminate();
    return;
  }

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Could not create window" << std::endl;
    glfwTerminate();
    return;
  }

  auto xpos = (videoMode->width - width) / 2;
  auto ypos = (videoMode->height - height) / 2;
  glfwSetWindowPos(window, xpos, ypos);
  glfwSetFramebufferSizeCallback(window, Resize);

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
  glClearColor(clear_color.red, clear_color.green, clear_color.blue, 1);
  glActiveTexture(GL_TEXTURE0);

  on_initialize();

  glfwShowWindow(window);

  auto last_update = glfwGetTime();
  auto delta = 0.0;
  while (!glfwWindowShouldClose(window)) {
    auto time = glfwGetTime();

    delta += time - last_update;
    last_update = time;
    while (delta >= update_rate) {
      delta -= update_rate;
      on_update(update_rate);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    on_render(time);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  on_terminate();

  glfwHideWindow(window);
  glfwDestroyWindow(window);
  glfwTerminate();
}

GLFWwindow *Window() {
  return window;
}

}
