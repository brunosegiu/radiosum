#include "IDRenderer.h"

#include "ConfigurationManager.h"
#include "HemicubeBuffer.h"

IDRenderer::IDRenderer(Scene* scene) {
	this->shader = new Shader("hemicube.vert", "hemicube.geom", "hemicube.frag");
	this->scene = scene;
	std::string width = ConfigurationManager::get("INTERNAL_WIDTH");
	std::string height = ConfigurationManager::get("INTERNAL_HEIGHT");
	this->camera = new Camera(GLuint(std::stoi(width)), GLuint(std::stoi(height)), 45.0f, 0.5f, 5000.0f);
	this->buffer = new HemicubeBuffer(std::stoi(ConfigurationManager::get("INTERNAL_WIDTH")), std::stoi(ConfigurationManager::get("INTERNAL_HEIGHT")));
}

void IDRenderer::render() {
	this->shader->bind();
	this->buffer->bind();
	this->buffer->clean();
	GLuint worldTransformID = glGetUniformLocation(shader->getID(), "worldTransform");
	glm::mat4 toWorldCoords = this->camera->getMVPMatrix();
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	this->scene->draw();
}

IDRenderer::~IDRenderer() {
	delete this->shader;
	delete this->camera;
}
