#include "display/BufferRenderer.h"

#include "common/ConfigurationManager.h"

BufferRenderer::BufferRenderer() {
	this->shader = new Shader("renderFromText.vert", "renderFromText.frag");
	GLuint height = std::stoi(ConfigurationManager::get("APP_RES_HEIGHT"));
	this->buffer = new Buffer(height, height);

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
	this->buffer->bind();
	this->buffer->clean();
	glBindVertexArray(this->GLQuadId);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

BufferRenderer::~BufferRenderer() {
	delete this->buffer;
}