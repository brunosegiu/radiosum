#include "shaders/ComputeShader.h"

#include <fstream>

#include "ConfigurationManager.h"
#include "EngineStore.h"
#include "Utils.h"

ComputeShader::ComputeShader(std::string name) {
  EngineStore::logger.log("Loading compute shader " + name);
  GLuint GLComputeId = 0;
  std::string program;
  std::string base = ConfigurationManager::get("SHADER_BASE_PATH");
  std::string fullpath = resolve(base + name);
  std::ifstream file(fullpath.c_str());
  if (file) {
    program.assign((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
    GLComputeId = glCreateShader(GL_COMPUTE_SHADER);
    const GLchar* shaderSource = program.c_str();
    glShaderSource(GLComputeId, 1, (const GLchar**)&shaderSource, NULL);
    glCompileShader(GLComputeId);
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(GLComputeId, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE) {
      throw std::runtime_error("Unable to compile shader");
      glDeleteShader(GLComputeId);
      GLComputeId = 0;
    }
  } else {
    throw std::runtime_error("Unable to open file " + fullpath);
  }
  GLProgramId = glCreateProgram();
  glAttachShader(GLProgramId, GLComputeId);
  this->link();
  glDeleteShader(GLComputeId);
}

void ComputeShader::bind() { glUseProgram(GLProgramId); }

void ComputeShader::unbind() { glUseProgram(0); }

GLuint ComputeShader::getID() { return this->GLProgramId; }

void ComputeShader::link() {
  glLinkProgram(GLProgramId);
  GLint shaderLinked = GL_FALSE;
  glGetProgramiv(GLProgramId, GL_LINK_STATUS, &shaderLinked);
  if (shaderLinked != GL_TRUE) {
    throw std::runtime_error("Unable to link shader");
  }
}

void ComputeShader::run(GLuint dimX, GLuint dimY, GLuint dimZ) {
  glDispatchCompute(dimX, dimY, dimZ);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

ComputeShader::~ComputeShader() { glDeleteProgram(GLProgramId); }
