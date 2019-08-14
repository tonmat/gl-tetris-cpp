#include <algorithm>
#include "game.h"

namespace game {

Game::Game(unsigned char boardWidth, unsigned char boardHeight) : board_(boardWidth, boardHeight) {
  Reset();
}

void Game::Reset() {
  inputs = {};
  game_over_ = false;
  time_ = 0.0f;
  clear_time_ = 0.0f;
  level_ = 1;
  count_ = 0;
  score_ = 0;
  fall_cd_ = 0.0f;
  clear_cd_ = 0.0f;
  clear_y0_ = 0;
  clear_y1_ = 0;
  can_hold_ = false;
  board_.Clear();
  player_.Clear();
  shadow_.Clear();
  temp_.Clear();
  next_.Random();
  hold_.Clear();
}

void Game::Update(float delta) {
  if (game_over_)
    return;
  time_ += delta;
  if (clear_cd_ > 0) {
    UpdateClear(delta);
  } else {
    UpdateRotate();
    UpdateMove();
    UpdateDrop();
    UpdateHold();
    UpdateFall(delta);
  }
  inputs = {};
}

void Game::UpdateClear(float delta) {
  if (clear_cd_ <= 0)
    return;
  clear_cd_ -= delta;
  if (clear_cd_ <= 0) {
    clear_cd_ = 0;
    for (char y = clear_y1_; y >= clear_y0_; y--)
      if (board_.IsFullRow(y))
        board_.RemoveRow(y);
    SetPlayer(next_);
    next_.Random();
    can_hold_ = true;
    UpdateShadow();
  }
}

void Game::UpdateRotate() {
  char direction = 0;
  if (inputs.rotate_ccw) direction--;
  if (inputs.rotate_cw) direction++;
  if (direction == 0)
    return;
  player_.Rotate(direction);
  if (board_.CheckPlayer(player_)) {
    UpdateShadow();
    return;
  }
  player_.Rotate(-direction);
}

void Game::UpdateMove() {
  char dx = 0;
  if (inputs.move_left) dx--;
  if (inputs.move_right) dx++;
  if (dx == 0)
    return;
  player_.x += dx;
  if (board_.CheckPlayer(player_)) {
    UpdateShadow();
    return;
  }
  player_.x -= dx;
}

void Game::UpdateDrop() {
  if (inputs.drop_soft || inputs.drop_hard)
    fall_cd_ = 0;
  if (inputs.drop_hard)
    player_.Set(shadow_);
}

void Game::UpdateHold() {
  if (!inputs.hold || !can_hold_)
    return;
  can_hold_ = false;
  temp_.Set(hold_);
  hold_.Set(player_);
  if (temp_.size() == 0) {
    SetPlayer(next_);
    next_.Random();
    UpdateShadow();
    return;
  }
  SetPlayer(temp_);
  UpdateShadow();
}

void Game::UpdateFall(float delta) {
  fall_cd_ -= delta;
  if (fall_cd_ <= 0) {
    player_.y--;
    if (board_.CheckPlayer(player_)) {
      fall_cd_ += next_fall_cd();
      return;
    }
    fall_cd_ = next_fall_cd();
    player_.y++;
    auto rows = board_.PlacePlayer(player_);
    if (rows == 0) {
      SetPlayer(next_);
      next_.Random();
      can_hold_ = true;
      UpdateShadow();
      if (!game_over_ && player_.size() > 0) {
        count_++;
        if (count_ % 8 == 0 && level_ < 99)
          level_++;
      }
      return;
    }
    score_ += 1u << rows * 2u;
    clear_cd_ = 0.8f;
    clear_time_ = time_;
    clear_y0_ = std::max(player_.y, '\0');
    clear_y1_ = std::min(player_.y + player_.size() - 1, board_.height() - 1);
    player_.Clear();
    UpdateShadow();
  }
}

void Game::UpdateShadow() {
  shadow_.Set(player_);
  if (shadow_.size() <= 0)
    return;
  while (true) {
    shadow_.y--;
    if (!board_.CheckPlayer(shadow_)) {
      shadow_.y++;
      break;
    }
  }
}

void Game::SetPlayer(Shape &shape) {
  player_.Set(shape);
  player_.x = (board_.width() - player_.size()) / 2;
  player_.y = board_.height() - 1;
  char tries = player_.size();
  while (tries-- > 0) {
    player_.y--;
    if (board_.CheckPlayer(player_, true))
      break;
  }
  if (!board_.CheckPlayer(player_)) {
    game_over_ = true;
    board_.PlacePlayer(player_);
    player_.Clear();
  }
}

}
