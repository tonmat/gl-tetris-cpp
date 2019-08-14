#ifndef GL_TETRIS_CPP_SRC_INPUTS_INPUTS_H_
#define GL_TETRIS_CPP_SRC_INPUTS_INPUTS_H_

struct GLFWwindow;

namespace inputs {

extern const int kP;
extern const int kR;
extern const int kLeft;
extern const int kRight;
extern const int kDown;
extern const int kUp;
extern const int kLeftControl;
extern const int kSpace;

class Inputs {
 public:
  explicit Inputs(GLFWwindow *window);

  void Update(float delta);
  bool isDown(int code, float repeat1, float repeatN);
  bool isJustPressed(int code);

 private:
  GLFWwindow *window_;
  float time_{0};
  float state_[512]{-1};
};

}

#endif
