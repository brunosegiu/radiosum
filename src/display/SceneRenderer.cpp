#include "display/SceneRenderer.h"

#include "common/ConfigurationManager.h"

SceneRenderer::SceneRenderer(Scene* scene) {
	this->shader = new Shader("final.vert", "final.frag");
	GLuint width = std::stoi(ConfigurationManager::get("APP_RES_WIDTH"));
	GLuint height = std::stoi(ConfigurationManager::get("APP_RES_HEIGHT"));
	this->buffer = new Buffer(width, height);
	this->scene = scene;
}

void SceneRenderer::render() {
	this->shader->bind();
	this->buffer->bind();
	this->buffer->clean();
	GLuint worldTransformID = glGetUniformLocation(shader->getID(), "worldTransform");
	glm::mat4 toWorldCoords = this->camera->getMVPMatrix();
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	this->scene->draw();
}

SceneRenderer::~SceneRenderer() {
	delete this->buffer;
	delete this->shader;
}