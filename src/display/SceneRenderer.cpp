#include "display/SceneRenderer.h"

#include "common/ConfigurationManager.h"
#include "common/buffers/PickingBuffer.h"
#include "common/Application.h"
#include "common/Logger.h"

SceneRenderer::SceneRenderer() {
	this->shader = new Shader("final.vert", "final.frag");
	GLuint width = std::stoi(ConfigurationManager::get("APP_WINDOW_WIDTH"));
	GLuint height = std::stoi(ConfigurationManager::get("APP_WINDOW_HEIGHT"));
	this->buffer = new PickingBuffer(width, height);
	this->camera = Application::getWindow()->getCamera();
	this->scene = nullptr;
}

void SceneRenderer::render() {
	if (this->scene != nullptr) {
		this->shader->bind();
		this->buffer->bind();
		this->buffer->clean();
		GLuint worldTransformID = glGetUniformLocation(shader->getID(), "worldTransform");
		glm::mat4 toWorldCoords = this->camera->getMVPMatrix();
		glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
		this->scene->draw();
	}
}

void SceneRenderer::read() {
	this->buffer->read();
}

SceneRenderer::~SceneRenderer() {
	delete this->buffer;
	delete this->shader;
}