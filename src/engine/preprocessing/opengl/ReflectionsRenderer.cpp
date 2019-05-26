#include "preprocessing/opengl/ReflectionsRenderer.h"

#include <gtc/type_ptr.hpp>

#include "ConfigurationManager.h"
#include "buffers/IDBuffer.h"

ReflectionsRenderer::ReflectionsRenderer(Scene *scene) {
  this->shader = new Shader("reflections.vert", "reflections.frag");
  this->stencilShader =
      new Shader("reflectionsStencil.vert", "reflectionsStencil.frag");
  this->scene = scene;
  std::string widthstr = ConfigurationManager::get("REFLECTIONS_WIDTH");
  GLuint width = std::stoi(widthstr);
  this->camera = nullptr;
  this->buffer = new IDBuffer(width);
}

void ReflectionsRenderer::render(Face &face, GLuint faceIndex,
                                 Face &reflectiveFace,
                                 GLuint reflectiveFaceIndex) {
  this->buffer->bind();
  dynamic_cast<IDBuffer *>(this->buffer)->cleanStencil();

  glm::vec3 viewPoint = face.getBarycenter();
  glm::vec3 reflectiveCenter = reflectiveFace.getBarycenter();
  GLfloat distance = glm::distance(viewPoint, reflectiveCenter);

  glm::vec3 direction = reflectiveFace.getNormal();
  glm::vec3 origin = reflectiveCenter - direction * distance;
  glm::vec3 up = reflectiveFace.getUp();

  Camera reflectionCamera =
      Camera(1.0f, 90.0f, 0.5f, 5000.0f, origin, direction, up);

  // Render stencil buffer
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilMask(0xFF);
  glDepthMask(GL_FALSE);
  this->stencilShader->bind();
  GLuint worldTransformId =
      glGetUniformLocation(shader->getID(), "worldTransform");
  glUniformMatrix4fv(worldTransformId, 1, GL_FALSE,
                     glm::value_ptr(reflectionCamera.getMVPMatrix()));

  this->scene->drawFace(faceIndex);

  // Render scene
  this->shader->bind();
  this->buffer->clean();
  glDepthMask(GL_TRUE);
  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilMask(0x00);
  glm::vec4 clipAt = reflectiveFace.getPlane();

  worldTransformId = glGetUniformLocation(shader->getID(), "worldTransform");
  glUniformMatrix4fv(worldTransformId, 1, GL_FALSE,
                     glm::value_ptr(reflectionCamera.getMVPMatrix()));

  glEnable(GL_CLIP_DISTANCE0);
  GLuint clipPlaneId = glGetUniformLocation(shader->getID(), "clipPlane");
  glUniform4fv(clipPlaneId, 1, glm::value_ptr(clipAt));

  this->scene->draw(this->shader->getID(), true);
  glDisable(GL_CLIP_DISTANCE0);
  glDisable(GL_STENCIL_TEST);
}

void ReflectionsRenderer::read() { this->buffer->read(); }

std::vector<GLuint> ReflectionsRenderer::getData() {
  return dynamic_cast<IDBuffer *>(this->buffer)->getData();
}

void ReflectionsRenderer::render() { throw std::exception("Not implemented"); }

ReflectionsRenderer::~ReflectionsRenderer() { delete this->shader; }
