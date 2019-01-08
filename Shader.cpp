#include "Shader.h"

#include <fstream>

void Shader::loadShader(GLenum type, GLuint &shaderID, std::string path) {
	std::string program;
	std::ifstream file(path.c_str());
	if (file) {
		program.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		shaderID = glCreateShader(type);
		const GLchar* shaderSource = program.c_str();
		glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);
		glCompileShader(shaderID);
		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE) {

			throw std::runtime_error("Unable to compile shader");
			glDeleteShader(shaderID);
			shaderID = 0;
		}
	}
	else {
		throw std::runtime_error("Unable to open file");
	}
}

Shader::Shader(std::string vertPath, std::string geomPath, std::string fragPath) {
	GLuint GLVertexId, GLgeomId, GLfragId;
	GLVertexId = GLfragId = GLgeomId = 0;
	loadShader(GL_VERTEX_SHADER, GLVertexId, vertPath);
	loadShader(GL_GEOMETRY_SHADER, GLgeomId, geomPath);
	loadShader(GL_FRAGMENT_SHADER, GLfragId, fragPath);
	GLProgramId = glCreateProgram();
	glAttachShader(GLProgramId, GLVertexId);
	glAttachShader(GLProgramId, GLgeomId);
	glAttachShader(GLProgramId, GLfragId);
	glLinkProgram(GLProgramId);
	glDeleteShader(GLVertexId);
	glDeleteShader(GLgeomId);
	glDeleteShader(GLfragId);
}

Shader::Shader(std::string vertPath, std::string fragPath) {
	GLuint GLVertexId, GLfragId;
	GLVertexId = GLfragId = 0;
	loadShader(GL_VERTEX_SHADER, GLVertexId, vertPath);
	loadShader(GL_FRAGMENT_SHADER, GLfragId, fragPath);
	GLProgramId = glCreateProgram();
	glAttachShader(GLProgramId, GLVertexId);
	glAttachShader(GLProgramId, GLfragId);
	this->link();
	glDeleteShader(GLVertexId);
	glDeleteShader(GLfragId);
}

void Shader::bind() {
	glUseProgram(GLProgramId);
}

void Shader::unbind() {
	glUseProgram(0);
}

GLuint Shader::getID() {
	return this->GLProgramId;
}

void Shader::link() {
	glLinkProgram(GLProgramId);
	GLint shaderLinked = GL_FALSE;
	glGetProgramiv(GLProgramId, GL_LINK_STATUS, &shaderLinked);
	if (shaderLinked != GL_TRUE) {
		throw std::runtime_error("Unable to link shader");
	}
}


Shader::~Shader() {
	glDeleteProgram(GLProgramId);
}
