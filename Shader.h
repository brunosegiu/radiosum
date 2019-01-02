#pragma once

#include <string>

#include <GL/glew.h>

class Shader {
private:
	GLuint GLProgramId, GLVertexId, GLfragId;
	void loadShader(GLenum type, GLuint &shaderID, std::string path);
public:
	Shader(std::string vertPath, std::string fragPath);
	void bind();
	void unbind();
	GLuint getID();
	virtual ~Shader();
};

