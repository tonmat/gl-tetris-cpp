
#include <graphics/graphics.h>
#include <graphics/shader.h>
#include <graphics/texture.h>
#include <graphics/batch.h>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 projection;
graphics::Shader *shader;
graphics::Texture *texture;
graphics::Sprite *sprite;
graphics::Batch *batch;

void Initialize() {
  projection = glm::ortho(0.0f, 15.0f, 0.0f, 20.0f);

  shader = new graphics::Shader({"shaders/shader.vert", "shaders/shader.frag"});
  shader->Bind();
  shader->SetUniform1i("u_material", 0);
  shader->SetUniformMat4("u_projection", projection);

  texture = new graphics::Texture("textures/spritesheet.png");
  texture->Bind();

  sprite = new graphics::Sprite{
      .texture_region = texture->Region(0, 64, 8, 8)
  };

  batch = new graphics::Batch(2048);
}

void Update(float delta) {

}

void Render(double time) {
  batch->Begin();
  batch->Draw(*sprite);
  batch->End();
}

int main() {
  graphics::Run({
                    .title = "gl_tetris_cpp",
                    .width = 600,
                    .height = 800,
                    .update_rate = 16,
                    .clear_color = {0, .025f, 0},
                    .on_initialize = Initialize,
                    .on_update = Update,
                    .on_render = Render,
                });
  return 0;
}