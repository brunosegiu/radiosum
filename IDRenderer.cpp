#include "IDRenderer.h"



IDRenderer::IDRenderer(std::vector<Mesh*> meshes, Camera* camera) {
	this->shader = new Shader("id.vert", "id.frag");
	this->meshes = meshes;
	this->camera = camera;
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
}
