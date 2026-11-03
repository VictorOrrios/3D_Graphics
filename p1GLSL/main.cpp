#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <IGL/BOX.h>
#include <IGL/IGlib.h>
#include <cstdlib>
#include <ctime>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.141592f

// Scene object IDs
int objId = -1;
int objId2 = -1;
int objId3 = -1;
float angle = 0.f;
float angle2 = 0.f;
float obj3T = 0.f;
const float obj3Radius = 5.f;
glm::vec3 obj3LastDir(1, 0, 0);
glm::vec3 obj3NextDir(0, 1, 0);

// Camera control
enum CameraControl { FPS = 0, ORBITAL };
const CameraControl cameraType = CameraControl::ORBITAL;
const float speed = 1.f;
const float sensitivity = PI / 1000.0f;
const glm::vec3 up(0, 1, 0);
// FPS Camera
glm::vec3 eye(0, 0, -10);
glm::vec3 center(0, 0, 1); // Relative
// ORBITAL Camera
const float orbitRadius = 10.0;
glm::vec3 target(0, 0, 0);
glm::vec3 orbitDir(0, 0, -1);
// Mouse vars
bool mousePressed = false;
glm::vec2 lastMousePos;

// CB declaration
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

void setProjMat(float aspect) {
  // Frustrum params
  float near = 0.01f;
  float far = 100.0f;
  float fov = 60;
  float tanInv = 1 / glm::tan((fov / 2.0) * PI / 180.0f);

  glm::mat4 projMat = glm::mat4(0.0f);
  projMat[0][0] = tanInv / aspect;
  projMat[1][1] = tanInv;
  projMat[2][2] = -(far + near) / (far - near);
  projMat[2][3] = -1.0f;
  projMat[3][2] = -(2.0f * far * near) / (far - near);
  IGlib::setProjMat(projMat);
}

void setViewMat() {
  glm::mat4 viewMat;
  if (cameraType == CameraControl::FPS) {
    viewMat = glm::lookAt(eye, eye + center, up);
  } else if (cameraType == CameraControl::ORBITAL) {
    viewMat = glm::lookAt(orbitDir * orbitRadius + target, target, up);
  }
  IGlib::setViewMat(viewMat);
}

int main(int argc, char **argv) {
#ifdef _WIN32
  std::locale::global(std::locale("spanish")); // Spanish accents
#endif
  std::string vertexShader = std::string(SHADERS_PATH) + "/shader.v10.vert";
  std::string fragmentShader = std::string(SHADERS_PATH) + "/shader.v10.frag";
  if (!IGlib::init(vertexShader.c_str(), fragmentShader.c_str()))
    return -1;

  // Adjust the camera
  // Default values if they are not defined
  // IGlib::setProjMat(const glm::mat4 &projMat);
  // IGlib::setViewMat(const glm::mat4 &viewMat);

  // Create the object we want to visualize
  objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
                           cubeVertexPos, cubeVertexColor, cubeVertexNormal,
                           cubeVertexTexCoord, cubeVertexTangent);
  objId2 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
                            cubeVertexPos, cubeVertexColor, cubeVertexNormal,
                            cubeVertexTexCoord, cubeVertexTangent);
  objId3 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
                            cubeVertexPos, cubeVertexColor, cubeVertexNormal,
                            cubeVertexTexCoord, cubeVertexTangent);

  glm::mat4 modelMat = glm::mat4(1.0f);
  IGlib::setModelMat(objId, modelMat);
  IGlib::setModelMat(objId2, modelMat);
  IGlib::setModelMat(objId3, modelMat);

  setViewMat();

  setProjMat(1);

  // Add textures here
  std::string colorTexturePath = std::string(TEXTURES_PATH) + "/color.png";
  IGlib::addColorTex(objId, colorTexturePath.c_str());
  IGlib::addColorTex(objId2, colorTexturePath.c_str());
  IGlib::addColorTex(objId3, colorTexturePath.c_str());

  // CBs
  IGlib::setResizeCB(resizeFunc);
  IGlib::setIdleCB(idleFunc);
  IGlib::setKeyboardCB(keyboardFunc);
  IGlib::setMouseCB(mouseFunc);
  IGlib::setMouseMoveCB(mouseMotionFunc);

  // Main loop
  IGlib::mainLoop();
  IGlib::destroy();
  return 0;
}

void resizeFunc(int width, int height) {
  // Adjust aspect ratio to window size
  setProjMat((float)width / (float)height);
}

void idleFunc() {
  glm::mat4 modelMat = glm::mat4(1.0f);
  modelMat = glm::rotate(modelMat, angle, glm::vec3(1, 1, 0));
  IGlib::setModelMat(objId, modelMat);

  glm::mat4 modelMat2 = glm::mat4(1.0f);
  modelMat2 = glm::rotate(modelMat2, angle2, glm::vec3(0, 1, 0));
  modelMat2 = glm::translate(modelMat2, glm::vec3(5, 0, 0));
  modelMat2 = glm::rotate(modelMat2, angle2, glm::vec3(0, 1, 0));
  IGlib::setModelMat(objId2, modelMat2);

  auto bezier = [](float t) {
    const float p0 = 0;
    const float p1 = 0.1;
    const float p2 = 0.9;
    const float p3 = 1;

    const float u = 1 - t;

    return (u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 +
            t * t * t * p3);
  };
  glm::mat4 modelMat3 = glm::mat4(1.0f);
  float bezierAlpha = bezier(obj3T);
  glm::vec3 interpolPos =
      obj3LastDir * (1 - bezierAlpha) + obj3NextDir * bezierAlpha;
  interpolPos = glm::normalize(interpolPos) * obj3Radius;
  modelMat3 = glm::translate(modelMat3, interpolPos);
  IGlib::setModelMat(objId3, modelMat3);

  angle = (angle > PI * 2.0) ? 0.0f : angle + 0.01;
  angle2 = (angle > PI * 2.0) ? 0.0f : angle + 0.01;

  auto r01 = []() { return (float(rand()) / RAND_MAX - 0.5f) * 2.0f; };
  if (obj3T > 1.0) {
    obj3T = 0.0f;
    obj3LastDir = obj3NextDir;
    obj3NextDir = glm::normalize(glm::vec3(r01(), r01(), r01()));
  } else {
    obj3T += 0.01;
  }
}

void keyboardFunc(unsigned char key, int x, int y) {
  if (cameraType == CameraControl::FPS) {
    switch (key) {
    case 'w':
      eye += center * speed;
      break;
    case 's':
      eye -= center * speed;
      break;
    case 'd': {
      glm::vec3 right = glm::cross(center, up);
      eye += right * speed;
      break;
    }
    case 'a': {
      glm::vec3 left = glm::cross(up, center);
      eye += left * speed;
      break;
    }
    }
  } else if (cameraType == CameraControl::ORBITAL) {
    glm::vec3 forward = glm::normalize(glm::vec3(-orbitDir.x, 0, -orbitDir.z));
    glm::vec3 right = glm::cross(forward, up);
    switch (key) {
    case 'w':
      target += forward * speed;
      break;
    case 's':
      target -= forward * speed;
      break;
    case 'd': {
      target += right * speed;
      break;
    }
    case 'a': {
      target -= right * speed;
      break;
    }
    }
  }

  setViewMat();
}

void mouseFunc(int button, int state, int x, int y) {
  if (state == 0) {
    if (button == 0) {
      mousePressed = true;
      lastMousePos = glm::vec2(x, y);
    }
  } else {
    if (button == 0)
      mousePressed = false;
  }
}

void mouseMotionFunc(int x, int y) {
  if (!mousePressed)
    return;

  glm::vec2 mouseDelta(x - lastMousePos.x, lastMousePos.y - y);

  if (cameraType == CameraControl::FPS) {
    glm::vec3 right = glm::cross(center, up);
    glm::vec3 localUp = glm::cross(right, center);
    glm::mat4 m(1.0f);
    m = glm::rotate(m, -mouseDelta.x * sensitivity, localUp);
    m = glm::rotate(m, mouseDelta.y * sensitivity, right);
    center = m * glm::vec4(center, 1);
  } else if (cameraType == CameraControl::ORBITAL) {
    glm::vec3 forward = -eye;
    glm::vec3 right = glm::cross(forward, up);
    glm::mat4 m(1.0f);
    m = glm::rotate(m, -mouseDelta.x * sensitivity, up);
    m = glm::rotate(m, mouseDelta.y * sensitivity, right);
    orbitDir = m * glm::vec4(orbitDir, 1);
  }

  lastMousePos.x = x;
  lastMousePos.y = y;
  setViewMat();
}
