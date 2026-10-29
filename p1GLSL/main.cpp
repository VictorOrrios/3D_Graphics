#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/trigonometric.hpp"
#include <IGL/BOX.h>
#include <IGL/IGlib.h>
#include <ctime>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.141592f

// Scene object IDs
int objId = -1;
float angle = 0.f;

// CB declaration
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

int main(int argc, char **argv)
{
#ifdef _WIN32
	std::locale::global(std::locale("spanish")); // Spanish accents
#endif
	std::string vertexShader = std::string(SHADERS_PATH) + "/shader.v5.vert";
	std::string fragmentShader = std::string(SHADERS_PATH) + "/shader.v5.frag";
	if (!IGlib::init(vertexShader.c_str(), fragmentShader.c_str()))
		return -1;

	// Adjust the camera
	// Default values if they are not defined
	// IGlib::setProjMat(const glm::mat4 &projMat);
	// IGlib::setViewMat(const glm::mat4 &viewMat);

	// Create the object we want to visualize
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
							 cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);

	glm::mat4 viewMat = glm::mat4(1.0f);
	viewMat[3].z = -6;
	IGlib::setViewMat(viewMat);

	// Frustrum params
	float near = 0.01f;
	float far = 10.0f;
	float fov = 60;
	float tanInv = 1/glm::tan((fov/2.0)*PI/180.0f);

	glm::mat4 projMat = glm::mat4(0.0f);
	projMat[0].x = tanInv;
	projMat[1].y = tanInv;
	projMat[2].z = -(far+near)/(far-near);
	projMat[2].w = -(2*far*near)/(far-near);
	projMat[3].z = -1;

	// Add textures here

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

void resizeFunc(int width, int height)
{
	// Adjust aspect ratio to window size
}

void idleFunc()
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, angle, glm::vec3(1,1,0));
    IGlib::setModelMat(objId, modelMat);
    angle = (angle>PI*2.0)? 0.0f : angle+0.01;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "The pressed key is " << key << std::endl
			  << std::endl;
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state == 0)
		std::cout << "The pressed button is ";
	else
		std::cout << "The released button is ";

	if (button == 0)
		std::cout << "left mouse button " << std::endl;
	if (button == 1)
		std::cout << "central mouse button " << std::endl;
	if (button == 2)
		std::cout << "right mouse button " << std::endl;

	std::cout << "in the window's position " << x << " " << y << std::endl
			  << std::endl;
}

void mouseMotionFunc(int x, int y)
{
}
