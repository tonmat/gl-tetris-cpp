#ifndef GL_TETRIS_CPP_SRC_GAME_GAME_H_
#define GL_TETRIS_CPP_SRC_GAME_GAME_H_

#include "board.h"
#include "player.h"
#include "input.h"

namespace game {

class Game {
 public:
  Game(unsigned char boardWidth, unsigned char boardHeight);
  virtual ~Game() = default;

  void Reset();
  void Update(float delta);

  Input inputs{};
  [[nodiscard]] float time() const { return time_; }
  [[nodiscard]] float clear_time() const { return clear_time_; }
  [[nodiscard]] unsigned char level() const { return level_; }
  [[nodiscard]] float fall_cd() const { return fall_cd_; }
  [[nodiscard]] float clear_cd() const { return clear_cd_; }
  [[nodiscard]]  Board const &board() const { return board_; }
  [[nodiscard]]  Player const &player() const { return player_; }
  [[nodiscard]]  Player const &shadow() const { return shadow_; }
  [[nodiscard]]  Shape const &next() const { return next_; }
  [[nodiscard]]  Shape const &hold() const { return hold_; }

 private:
  void UpdateClear(float delta);
  void UpdateRotate();
  void UpdateMove();
  void UpdateDrop();
  void UpdateHold();
  void UpdateFall(float delta);
  void UpdateShadow();

  bool SetPlayer(Shape &shape);

  [[nodiscard]] float next_fall_cd() const { return 0.1f + 0.9f * (99.0f - static_cast<float>(level_)) / 98.0f; };

  float time_;
  float clear_time_;
  unsigned char level_;
  float fall_cd_;
  float clear_cd_;
  char clear_y0_;
  char clear_y1_;
  bool can_hold_;
  Board board_;
  Player player_;
  Player shadow_;
  Player temp_;
  Shape next_{0, 0};
  Shape hold_{0, 0};
};

}

#endif
