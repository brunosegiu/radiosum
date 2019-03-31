#pragma once

#include <string>

#include <GL/glew.h>

class Shader {
 public:
  Shader(std::string vertName, std::string fragName);
  Shader(std::string vertName, std::string geomName, std::string fragName);
  void bind();
  void unbind();
  GLuint getID();
  virtual ~Shader();

 private:
  GLuint GLProgramId, GLVertexId, GLfragId;
  ;
  void loadShader(GLenum type, GLuint &shaderID, std::string path);
  void link();
};
