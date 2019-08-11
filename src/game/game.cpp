#include "game.h"

namespace game {

Game::Game(unsigned char boardWidth, unsigned char boardHeight) {
  time_ = 0;
  board_ = new Board(boardWidth, boardHeight);
  player_ = new Player();
  next_ = new Shape(3, 0b010111000);
}

Game::~Game() {
  delete board_;
  delete player_;
  delete next_;
}

void Game::Update(float delta) {
  time_ += delta;
}

}
