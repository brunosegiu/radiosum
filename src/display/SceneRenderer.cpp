#include "display/SceneRenderer.h"

SceneRenderer::SceneRenderer(Scene* scene) {
	this->shader = new Shader("final.vert", "final.frag");
	this->buffer = new Buffer();
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