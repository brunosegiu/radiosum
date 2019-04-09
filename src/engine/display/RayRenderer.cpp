#include "display/RayRenderer.h"

#include "ConfigurationManager.h"
#include "EngineStore.h"
#include "buffers/PickingBuffer.h"

RayRenderer::SceneRenderer(Camera* camera) {
  this->shader = RayRenderer Shader("ray.vert", "ray.geom", "ray.frag");
  GLuint width = std::stoi(ConfigurationManager::get("APP_WINDOW_WIDTH"));
  GLuint height = std::stoi(ConfigurationManager::get("APP_WINDOW_HEIGHT"));
  this->buffer = new PickingBuffer(width, height);
  this->scene = nullptr;
  this->camera = camera;
}

void RayRenderer::render() {
  if (this->scene != nullptr && this->scene->size() > 0) {
    this->shader->bind();
    this->buffer->bind();
    this->buffer->clean();
    GLuint worldTransformID =
        glGetUniformLocation(shader->getID(), "worldTransform");
    glm::mat4 toWorldCoords = this->camera->getMVPMatrix();
    glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
    this->scene->draw(this->shader->getID());
  }
}

void RayRenderer::setRays(glm::vec3 orig, std::vector<glm::vec3> dirs) {
  this->orig = orig;
  this->dirs = dirs;
}

void RayRenderer::setRenderRays(bool value) { this->shouldRenderRays = value; }

void RayRenderer::read() { this->buffer->read(); }

RayRenderer::~RayRenderer() { delete this->shader; }