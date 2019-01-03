#include "IDRenderer.h"

#include "ConfigurationManager.h"

IDRenderer::IDRenderer(std::vector<Mesh*> meshes) {
	this->shader = new Shader("id.vert", "id.frag");
	this->meshes = meshes;
	std::string width = ConfigurationManager::get("INTERNAL_WIDTH");
	std::string height = ConfigurationManager::get("INTERNAL_HEIGHT");
	this->camera = new Camera(GLuint(std::stoi(width)), GLuint(std::stoi(height)), 45.0f, 0.5f, 5000.0f);
}

void IDRenderer::render() {
	this->shader->bind();
	GLuint worldTransformID = glGetUniformLocation(shader->getID(), "worldTransform");
	glm::mat4 toWorldCoords = this->camera->getMVPMatrix();
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	for (auto mesh : meshes) {
		mesh->draw();
	}
}

IDRenderer::~IDRenderer() {
	delete this->shader;
	delete this->camera;
}
