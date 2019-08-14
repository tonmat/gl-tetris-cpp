
#include <graphics/graphics.h>
#include <graphics/shader.h>
#include <graphics/texture.h>
#include <graphics/batch.h>
#include <inputs/inputs.h>
#include <glm/gtc/matrix_transform.hpp>
#include <game/game.h>

static const unsigned char kBoardWidth = 10;
static const unsigned char kBoardHeight = 20;
static const float kBoardX = 5.0f;

glm::mat4 projection;
graphics::Shader *shader;
graphics::Texture *texture;
graphics::Sprite *spt_board;
graphics::Sprite *spt_game_over_overlay;
graphics::Sprite *spt_shape;
graphics::Sprite *spt_shape_locked;
graphics::Sprite *spt_piece_bg;
graphics::Sprite *spt_piece_fg;
graphics::Sprite *spt_clear;
graphics::Sprite *spt_player;
graphics::Sprite *spt_shadow;
graphics::Label *lbl_time;
graphics::Label *lbl_time_value;
graphics::Label *lbl_score;
graphics::Label *lbl_score_value;
graphics::Label *lbl_level;
graphics::Label *lbl_level_value;
graphics::Label *lbl_next;
graphics::Label *lbl_hold;
graphics::Label *lbl_game_over;
graphics::Batch *batch;

inputs::Inputs *in;

game::Game *g;

bool paused;

void Initialize() {
  projection = glm::ortho(0.0f, static_cast<float>(kBoardWidth) + kBoardX, 0.0f, static_cast<float>(kBoardHeight));

  shader = new graphics::Shader({"shaders/shader.vert", "shaders/shader.frag"});
  shader->Bind();
  shader->SetUniform1i("u_material", 0);
  shader->SetUniformMat4("u_projection", projection);

  texture = new graphics::Texture("textures/spritesheet.png");
  texture->Bind();

  spt_board = new graphics::Sprite{
      .x = kBoardX,
      .width = kBoardWidth,
      .height = kBoardHeight,
      .texture_region = texture->Region(0, 64, 1, 1)
  };
  spt_game_over_overlay = new graphics::Sprite{
      .color = {0.08f, 0.1f, 0.08f},
      .alpha = 0.96f,
      .texture_region = texture->Region(0, 64, 1, 1)
  };
  spt_shape = new graphics::Sprite{
      .x = kBoardX,
      .width = 4.0f,
      .height = 4.0f,
      .color = {0.9f, 0.95f, 0.9f},
      .texture_region = texture->Region(0, 64, 1, 1)
  };
  spt_shape_locked = new graphics::Sprite{
      .x = kBoardX,
      .width = 4.0f,
      .height = 4.0f,
      .color = {0.2f, 0.3f, 0.2f},
      .alpha = 0.75f,
      .texture_region = texture->Region(0, 64, 1, 1)
  };

  spt_piece_bg = new graphics::Sprite{
      .x = kBoardX,
      .color = {0.975f, 0.96f, 0.975f},
      .texture_region = texture->Region(0, 64, 8, 8)
  };
  spt_piece_fg = new graphics::Sprite{
      .x = kBoardX,
      .color = {0, 0.1f, 0},
      .texture_region = texture->Region(0, 64, 8, 8)
  };
  spt_clear = new graphics::Sprite{
      .x = kBoardX,
      .texture_region = texture->Region(0, 64, 8, 8)
  };
  spt_player = new graphics::Sprite{
      .x = kBoardX,
      .texture_region = texture->Region(0, 64, 8, 8)
  };
  spt_shadow = new graphics::Sprite{
      .x = kBoardX,
      .color = {.6f, .8f, .6f},
      .texture_region = texture->Region(0, 64, 8, 8)
  };

  lbl_time = new graphics::Label{
      .text = "TIME",
      .x = kBoardX - 0.5f,
      .y = kBoardHeight - 2.0f,
      .size = 0.5f,
      .h_align = HAlign::kRight,
      .color = {0.5f, 0.6f, 0.5f}
  };
  lbl_time_value = new graphics::Label{
      .text = "",
      .x = kBoardX - 0.5f,
      .y = kBoardHeight - 3.0f,
      .h_align = HAlign::kRight,
      .color = {0.9f, 1.0f, 0.9f}
  };
  lbl_score = new graphics::Label{
      .text = "SCORE",
      .x = kBoardX - 0.5f,
      .y = kBoardHeight - 4.0f,
      .size = 0.5f,
      .h_align = HAlign::kRight,
      .color = {0.5f, 0.6f, 0.5f}
  };
  lbl_score_value = new graphics::Label{
      .text = "",
      .x = kBoardX - 0.5f,
      .y = kBoardHeight - 5.0f,
      .h_align = HAlign::kRight,
      .color = {0.9f, 1.0f, 0.9f}
  };
  lbl_level = new graphics::Label{
      .text = "LEVEL",
      .x = kBoardX - 0.5f,
      .y = kBoardHeight - 6.0f,
      .size = 0.5f,
      .h_align = HAlign::kRight,
      .color = {0.5f, 0.6f, 0.5f}
  };
  lbl_level_value = new graphics::Label{
      .text = "",
      .x = kBoardX - 0.5f,
      .y = kBoardHeight - 7.0f,
      .h_align = HAlign::kRight,
      .color = {0.9f, 1.0f, 0.9f}
  };
  lbl_next = new graphics::Label{
      .text = "NEXT",
      .x = kBoardX - 5.0f,
      .y = 5.25f,
      .size = 0.75f,
      .h_align = HAlign::kLeft,
      .color = {0.5f, 0.6f, 0.5f}
  };
  lbl_hold = new graphics::Label{
      .text = "HOLD",
      .x = kBoardX - 5.0f,
      .y = 11.5f,
      .size = 0.75f,
      .h_align = HAlign::kLeft,
      .color = {0.5f, 0.6f, 0.5f}
  };
  lbl_game_over = new graphics::Label{
      .text = "GAME\nOVER",
      .x = kBoardX + kBoardWidth / 2.0f,
      .y = 1.0f + kBoardHeight / 2.0f,
      .v_spacing = 1.0f,
      .size = 3.0f,
      .h_align = HAlign::kCenter,
      .color = {0.5f, 1.0f, 0.5f}
  };

  batch = new graphics::Batch(2048);

  in = new inputs::Inputs(graphics::Window());

  g = new game::Game(kBoardWidth, kBoardHeight);
}

void Terminate() {
  delete g;

  delete in;

  delete batch;
  delete spt_board;
  delete spt_game_over_overlay;
  delete spt_shape;
  delete spt_shape_locked;
  delete spt_piece_bg;
  delete spt_piece_fg;
  delete spt_player;
  delete spt_shadow;
  delete lbl_time;
  delete lbl_time_value;
  delete lbl_score;
  delete lbl_score_value;
  delete lbl_level;
  delete lbl_level_value;
  delete lbl_next;
  delete lbl_hold;
  delete lbl_game_over;
  delete texture;
  delete shader;
}

void Update(float delta) {
  in->Update(delta);

  if (in->isDown(inputs::kP, 1, 1)) paused = !paused;
  if (in->isDown(inputs::kR, 1, 1)) {
    g->Reset();
    paused = false;
  }

  if (paused)
    return;

  if (in->isDown(inputs::kLeft, 0.16f, 0.04f)) g->inputs.move_left = true;
  if (in->isDown(inputs::kRight, 0.16f, 0.04f)) g->inputs.move_right = true;
  if (in->isDown(inputs::kDown, 0.16f, 0.04f)) g->inputs.drop_soft = true;
  if (in->isDown(inputs::kUp, 1, 1)) g->inputs.drop_hard = true;
  if (in->isDown(inputs::kLeftControl, 1, 1)) g->inputs.hold = true;
  if (in->isDown(inputs::kSpace, 0.16f, 0.04f)) g->inputs.rotate_cw = true;

  g->Update(delta);
}

void RenderBoard() {
  auto &b = g->board();
  batch->Draw(*spt_board);
  for (unsigned char y = 0; y < b.height(); y++) {
    auto is_full_row = b.IsFullRow(y);
    for (unsigned char x = 0; x < b.width(); x++) {
      graphics::Sprite *spt;
      if (paused) {
        spt = spt_piece_fg;
      } else if (is_full_row) {
        spt = spt_clear;
      } else if (b[y].HasCell(x)) {
        spt = spt_piece_fg;
      } else {
        spt = spt_piece_bg;
      }
      spt->x = kBoardX + x;
      spt->y = y;
      batch->Draw(*spt);
    }
  }
}

void RenderPlayer(game::Player const &p, graphics::Sprite *spt) {
  for (unsigned char y = 0; y < p.size(); y++) {
    for (unsigned char x = 0; x < p.size(); x++) {
      if (!p.HasCell(x, y))
        continue;
      spt->x = kBoardX + p.x + x;
      spt->y = p.y + y;
      batch->Draw(*spt);
    }
  }
}

void RenderShape(game::Shape const &s, float offset_x, float offset_y) {
  spt_shape->x = offset_x + kBoardX;
  spt_shape->y = offset_y;
  batch->Draw(*spt_shape);
  if (s.size() > 0) {
    offset_x += 2.0f - s.center_x();
    offset_y += 2.0f - s.center_y();
  }
  for (unsigned char y = 0; y < s.size(); y++) {
    for (unsigned char x = 0; x < s.size(); x++) {
      if (!s.HasCell(x, y))
        continue;
      spt_piece_fg->x = offset_x + kBoardX + x;
      spt_piece_fg->y = offset_y + y;
      batch->Draw(*spt_piece_fg);
    }
  }
}

void RenderShapeLocked(float offset_x, float offset_y) {
  spt_shape_locked->x = offset_x + kBoardX;
  spt_shape_locked->y = offset_y;
  batch->Draw(*spt_shape_locked);
}

void RenderGameOverOverlay(float x, float y, float width, float height) {
  spt_game_over_overlay->x = x + kBoardX;
  spt_game_over_overlay->y = y;
  spt_game_over_overlay->width = width;
  spt_game_over_overlay->height = height;
  batch->Draw(*spt_game_over_overlay);
}

void RenderTime() {
  sprintf(lbl_time_value->text, "%01.0f:%02.0f", g->time() / 60, fmod(g->time(), 60));
  batch->Draw(*lbl_time);
  batch->Draw(*lbl_time_value);
}

void RenderScore() {
  sprintf(lbl_score_value->text, "%01d", g->score());
  batch->Draw(*lbl_score);
  batch->Draw(*lbl_score_value);
}

void RenderLevel() {
  sprintf(lbl_level_value->text, "%01d", g->level());
  batch->Draw(*lbl_level);
  batch->Draw(*lbl_level_value);
}

void RenderNext() {
  batch->Draw(*lbl_next);
}

void RenderHold() {
  batch->Draw(*lbl_hold);
}

void RenderGameOver() {
  batch->Draw(*lbl_game_over);
}

void Render(double time) {
  float i1 = std::abs(std::cos(0.7 * time));
  float i2 = std::abs(std::sin(1.7 * time));
  float i3 = 4 * (g->time() - g->clear_time());
  float i4;

  spt_player->color = {.4f, .1f * i1 + .5f, .4f};
  spt_shadow->alpha = .4f + .1f * i2;

  if (i3 < .5f) {
    i4 = i3 * 2;
    spt_clear->color = {.5f * i4, .1f + .9f * i4, .5f * i4};
    spt_clear->alpha = 1;
  } else {
    i4 = std::max(0.0f, 1.5f - i3);
    spt_clear->color = {.5f, 1.0f, .5f};
    spt_clear->alpha = i4;
  };

  batch->Begin();
  RenderBoard();
  if (!paused) {
    RenderPlayer(g->shadow(), spt_shadow);
    RenderPlayer(g->player(), spt_player);
    RenderShape(g->next(), -5.0f, 1.0f);
    RenderShape(g->hold(), -5.0f, 7.0f);
    if (!g->can_hold())
      RenderShapeLocked(-5.0f, 7.0f);
  }
  RenderTime();
  RenderScore();
  RenderLevel();
  RenderNext();
  RenderHold();
  if (g->game_over()) {
    RenderGameOverOverlay(-5.0f, 1.0f, 4.0f, 4.0f);
    RenderGameOverOverlay(-5.0f, 7.0f, 4.0f, 4.0f);
    RenderGameOverOverlay(0.0f, 0.0f, kBoardWidth, kBoardHeight);
    RenderGameOver();
  }
  batch->End();
}

int main() {
  graphics::Run({
                    .title = "gl_tetris_cpp",
                    .width = 40 * (kBoardWidth + static_cast<int>(kBoardX)),
                    .height = 40 * kBoardHeight,
                    .update_rate = 0.016f,
                    .clear_color = {0, .025f, 0},
                    .on_initialize = Initialize,
                    .on_terminate = Terminate,
                    .on_update = Update,
                    .on_render = Render,
                });
  return 0;
}