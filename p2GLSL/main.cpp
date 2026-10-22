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
	std::string vertexShader = std::string(SHADERS_PATH) + "/shader.v0.vert";
	std::string fragmentShader = std::string(SHADERS_PATH) + "/shader.v0.frag";
	if (!IGlib::init(vertexShader.c_str(), fragmentShader.c_str()))
		return -1;

	// Adjust the camera
	// Default values if they are not defined
	glm::mat4 view = glm::mat4(1.0);
	view[3].z = -6;
	IGlib::setViewMat(view);

	// Adjust the projection
	glm::mat4 proj = glm::mat4(1.0);
	float f = 1.0f / tan(PI / 6.0f);
	float far = 10.0f;
	float near = 0.1f;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near) / (near - far);
	proj[2].w = -1.0f;
	proj[3].z = (2.0f * far * near) / (near - far);
	proj[3].w = 0.0f;
	IGlib::setProjMat(proj);

	// Create the object we want to visualize
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
							 cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);

	// Add textures here
	std::string albedoTexPath = std::string(TEXTURES_PATH) + "/color.png";
	IGlib::addColorTex(objId, albedoTexPath.c_str());

	// CBs
	IGlib::setIdleCB(idleFunc);
	IGlib::setResizeCB(resizeFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);
	IGlib::setMouseMoveCB(mouseMotionFunc);

	// Mainloop
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
	glm::mat4 modelMat(1.0f);
	static float angle = 0.0f;
	angle = (angle > PI * 2.0f) ? 0 : angle + 0.01f;

	modelMat = glm::rotate(modelMat, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	IGlib::setModelMat(objId, modelMat);
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
