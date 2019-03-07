#include "display/TextureRenderer.h"

#include "common/ConfigurationManager.h"
#include "common/buffers/ColorBuffer.h"

TextureRenderer::TextureRenderer(std::string fragName) {
	this->shader = new Shader("renderFromText.vert", fragName);
	GLuint width = std::stoi(ConfigurationManager::get("APP_WINDOW_WIDTH"));
	GLuint height = std::stoi(ConfigurationManager::get("APP_WINDOW_HEIGHT"));
	this->buffer = new ColorBuffer(width, height);

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

void TextureRenderer::render() {
	this->shader->bind();
	this->buffer->bind();
	this->buffer->clean();
	glBindVertexArray(this->GLQuadId);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	this->buffer->unbind();
	glClear(GL_COLOR_BUFFER_BIT);
}

GLuint TextureRenderer::read() {
	return this->buffer->read();
}

TextureRenderer::~TextureRenderer() {
	delete this->buffer;
}