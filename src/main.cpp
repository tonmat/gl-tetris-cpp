
#include <graphics/graphics.h>
#include <graphics/shader.h>
#include <graphics/texture.h>
#include <graphics/batch.h>
#include <inputs/inputs.h>
#include <glm/gtc/matrix_transform.hpp>
#include <game/game.h>

static const float kBoardX = 5.0f;

glm::mat4 projection;
graphics::Shader *shader;
graphics::Texture *texture;
graphics::Sprite *spt_board;
graphics::Sprite *spt_piece_bg;
graphics::Sprite *spt_piece_fg;
graphics::Sprite *spt_clear;
graphics::Sprite *spt_player;
graphics::Sprite *spt_shadow;
graphics::Batch *batch;

inputs::Inputs *in;

game::Game *g;

bool paused;

void Initialize() {
  projection = glm::ortho(0.0f, 15.0f, 0.0f, 20.0f);

  shader = new graphics::Shader({"shaders/shader.vert", "shaders/shader.frag"});
  shader->Bind();
  shader->SetUniform1i("u_material", 0);
  shader->SetUniformMat4("u_projection", projection);

  texture = new graphics::Texture("textures/spritesheet.png");
  texture->Bind();

  spt_board = new graphics::Sprite{
      .x = kBoardX,
      .width = 10.0f,
      .height = 20.0f,
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

  batch = new graphics::Batch(2048);

  in = new inputs::Inputs(graphics::Window());

  g = new game::Game(10, 20);
}

void Terminate() {
  delete g;

  delete in;

  delete batch;
  delete spt_board;
  delete spt_piece_bg;
  delete spt_piece_fg;
  delete spt_player;
  delete spt_shadow;
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
      if (is_full_row) {
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
  }
  batch->End();
}

int main() {
  graphics::Run({
                    .title = "gl_tetris_cpp",
                    .width = 600,
                    .height = 800,
                    .update_rate = 0.016f,
                    .clear_color = {0, .025f, 0},
                    .on_initialize = Initialize,
                    .on_terminate = Terminate,
                    .on_update = Update,
                    .on_render = Render,
                });
  return 0;
}