#include <IGL/BOX.h>
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.141592f

// Scene object IDs
int objId = -1;

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
	std::string vertexShader = std::string(SHADERS_PATH) + "/shader.v1.vert";
	std::string fragmentShader = std::string(SHADERS_PATH) + "/shader.v1.frag";
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
