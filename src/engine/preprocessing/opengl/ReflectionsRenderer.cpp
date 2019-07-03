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

  glm::vec3 viewPoint = face.getBarycenter();
  glm::vec3 reflectiveCenter = reflectiveFace.getBarycenter();
  GLfloat distance = glm::distance(viewPoint, reflectiveCenter);

  glm::vec3 direction =
      2 * glm::dot(-face.getNormal(), reflectiveFace.getNormal()) -
      face.getNormal();
  glm::vec3 origin = reflectiveCenter - direction * distance;
  glm::vec3 up = reflectiveFace.getUp();

  Camera reflectionCamera =
      Camera(1.0f, 120.0f, 0.5f, 5000.0f, origin, direction, up);

  // Render stencil buffer
  glEnable(GL_STENCIL_TEST);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilMask(0xFF);
  glClear(GL_STENCIL_BUFFER_BIT);

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  this->stencilShader->bind();
  GLuint worldTransformId =
      glGetUniformLocation(stencilShader->getID(), "worldTransform");
  glUniformMatrix4fv(worldTransformId, 1, GL_FALSE,
                     glm::value_ptr(reflectionCamera.getMVPMatrix()));
  this->scene->drawFace(this->stencilShader->getID(), faceIndex);

  glEnable(GL_CULL_FACE);

  // Render scene
  this->shader->bind();
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glStencilMask(0x00);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilFunc(GL_EQUAL, 1, 0xFF);

  worldTransformId = glGetUniformLocation(shader->getID(), "worldTransform");
  glUniformMatrix4fv(worldTransformId, 1, GL_FALSE,
                     glm::value_ptr(reflectionCamera.getMVPMatrix()));

  glEnable(GL_CLIP_DISTANCE0);
  glm::vec4 clipAt = reflectiveFace.getPlane();
  GLuint clipPlaneId = glGetUniformLocation(shader->getID(), "clipPlane");
  glUniform4fv(clipPlaneId, 1, glm::value_ptr(clipAt));

  this->scene->draw(this->shader->getID(), true);
  glDisable(GL_CLIP_DISTANCE0);
  glEnable(GL_CULL_FACE);
  glDisable(GL_STENCIL_TEST);
  this->buffer->unbind();
}

std::vector<GLuint> ReflectionsRenderer::getData() {
  return dynamic_cast<IDBuffer *>(this->buffer)->getData();
}

void ReflectionsRenderer::render() { throw std::exception("Not implemented"); }

ReflectionsRenderer::~ReflectionsRenderer() { delete this->shader; }
