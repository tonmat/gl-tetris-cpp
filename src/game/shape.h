#ifndef GL_TETRIS_CPP_SRC_GAME_SHAPE_H_
#define GL_TETRIS_CPP_SRC_GAME_SHAPE_H_

namespace game {

class Shape {
 public:
  Shape(unsigned char size, unsigned short mask);
  virtual ~Shape() = default;

  virtual void Set(Shape &shape);
  bool HasCell(unsigned char x, unsigned char y);

  void RotateCW();
  void RotateCCW();

  [[nodiscard]] unsigned char size() const { return size_; }
  [[nodiscard]] unsigned short mask() const { return mask_; }
  [[nodiscard]] float center_x() const { return center_x_; }
  [[nodiscard]] float center_y() const { return center_y_; }

 private:
  void CalculateCenter();

  unsigned char size_;
  unsigned short mask_;
  float center_x_;
  float center_y_;
};

}

#endif
