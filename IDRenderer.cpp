#include "IDRenderer.h"

#include "ConfigurationManager.h"
#include "HemicubeBuffer.h"

#include <gtc/type_ptr.hpp>

IDRenderer::IDRenderer(Scene* scene) {
	this->shader = new Shader("hemicube.vert", "hemicube.geom", "hemicube.frag");
	this->scene = scene;
	std::string widthstr = ConfigurationManager::get("INTERNAL_WIDTH");
	GLuint width = std::stoi(widthstr);
	this->camera = new Camera(width, width, 45.0f, 0.5f, 5000.0f);
	this->buffer = new HemicubeBuffer(width);
}

void IDRenderer::render() {
	this->shader->bind();
	this->buffer->bind();
	this->buffer->clean();
	GLuint worldTransformID = glGetUniformLocation(shader->getID(), "worldTransform");
	std::vector<glm::mat4> toWorldCoords = this->camera->getCubeMatrices();
	glUniformMatrix4fv(worldTransformID, toWorldCoords.size(), GL_FALSE, glm::value_ptr(toWorldCoords[0]));
	this->scene->draw();
}

IDRenderer::~IDRenderer() {
	delete this->shader;
	delete this->camera;
}
