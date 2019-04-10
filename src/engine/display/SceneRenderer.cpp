#include "display/SceneRenderer.h"

#include "ConfigurationManager.h"
#include "EngineStore.h"
#include "buffers/PickingBuffer.h"

SceneRenderer::SceneRenderer(Camera* camera) {
  this->shader = new Shader("final.vert", "final.frag");
  GLuint width = std::stoi(ConfigurationManager::get("APP_WINDOW_WIDTH"));
  GLuint height = std::stoi(ConfigurationManager::get("APP_WINDOW_HEIGHT"));
  this->buffer = new PickingBuffer(width, height);
  this->scene = nullptr;
  this->camera = camera;
}

void SceneRenderer::render() {
  if (this->scene != nullptr && this->scene->size() > 0) {
    this->shader->bind();
    this->buffer->bind();
    this->buffer->clean();
    GLuint worldTransformID =
        glGetUniformLocation(shader->getID(), "worldTransform");
    glm::mat4 toWorldCoords = this->camera->getMVPMatrix();
    glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
    GLuint radiosityScaleID =
        glGetUniformLocation(shader->getID(), "radiosityScale");
    glUniform1f(radiosityScaleID, EngineStore::radiosityScale);
    this->scene->draw(this->shader->getID());
  }
}

void SceneRenderer::read() { this->buffer->read(); }

SceneRenderer::~SceneRenderer() {
  delete this->buffer;
  delete this->shader;
}