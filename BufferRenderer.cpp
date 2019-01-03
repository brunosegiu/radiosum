#include "BufferRenderer.h"

#include "ConfigurationManager.h"

BufferRenderer::BufferRenderer() {
	this->shader = new Shader(ConfigurationManager::get("DEBUG_SHADER_VERT"), ConfigurationManager::get("DEBUG_SHADER_FRAG"));

	static const GLfloat quad[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
	};

	this->GLQuadId = this->GLVerticesId = 0;

	glGenBuffers(1, &this->GLVerticesId);
	glBindBuffer(GL_ARRAY_BUFFER, this->GLVerticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	//Set up the VAO
	glGenVertexArrays(1, &this->GLQuadId);
	glBindVertexArray(this->GLQuadId);

	//Bind Vertices
	glBindBuffer(GL_ARRAY_BUFFER, this->GLVerticesId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void BufferRenderer::render() {
	this->shader->bind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLuint textureBufferID = glGetUniformLocation(shader->getID(), "buffer");
	glUniform1i(textureBufferID, 0);
	glBindVertexArray(this->GLQuadId);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

BufferRenderer::~BufferRenderer() {

}