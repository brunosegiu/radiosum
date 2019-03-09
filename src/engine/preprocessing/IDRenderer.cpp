#include "preprocessing/IDRenderer.h"

#include <gtc/type_ptr.hpp>

#include "ConfigurationManager.h"
#include "buffers/HemicubeBuffer.h"

IDRenderer::IDRenderer(Scene* scene) {
	this->shader = new Shader("hemicube.vert", "hemicube.geom", "hemicube.frag");
	this->scene = scene;
	std::string widthstr = ConfigurationManager::get("INTERNAL_WIDTH");
	GLuint width = std::stoi(widthstr);
	this->camera = nullptr;
	this->buffer = new HemicubeBuffer(width);
}

void IDRenderer::render() {
	this->shader->bind();
	this->buffer->bind();
	this->buffer->clean();
	GLuint worldTransformId = glGetUniformLocation(shader->getID(), "worldTransform");
	std::vector<glm::mat4> toWorldCoords = this->camera->getCubeMatrices();
	glUniformMatrix4fv(worldTransformId, toWorldCoords.size(), GL_FALSE, glm::value_ptr(toWorldCoords[0]));

	glEnable(GL_CLIP_DISTANCE0);
	GLuint clipPlaneId = glGetUniformLocation(shader->getID(), "clipPlane");
	glUniform4fv(clipPlaneId, 1, glm::value_ptr(this->clipPlane));

	this->scene->draw();
	glDisable(GL_CLIP_DISTANCE0);
	this->buffer->unbind();
}

void IDRenderer::read() {
	glFlush();
	this->buffer->read();
}

void IDRenderer::setClipPlane(glm::vec4 plane) {
	this->clipPlane = plane;
}

IDRenderer::~IDRenderer() {
	delete this->shader;
}
