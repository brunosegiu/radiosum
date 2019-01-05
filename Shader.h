#pragma once

#include <string>

#include <GL/glew.h>

class Shader {
public:
	Shader(std::string vertPath, std::string fragPath);
	Shader(std::string vertPath, std::string geomPath, std::string fragPath);
	void bind();
	void unbind();
	GLuint getID();
	virtual ~Shader();
private:
	GLuint GLProgramId, GLVertexId, GLfragId;;
	void loadShader(GLenum type, GLuint &shaderID, std::string path);
	void link();
};

