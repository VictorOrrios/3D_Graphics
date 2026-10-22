#include <IGL/BOX.h>
#include <IGL/auxiliar.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#define PI 3.141592f

//////////////////////////////////////////////////////////////
// Data stored in CPU memory
//////////////////////////////////////////////////////////////

// Window
GLFWwindow *window = NULL;

// Matrices
glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);

//////////////////////////////////////////////////////////////
// Variables to access OpenGL Objects
//////////////////////////////////////////////////////////////
// TO BE DEFINED

//////////////////////////////////////////////////////////////
// Auxiliar Functions
//////////////////////////////////////////////////////////////
// TO BE IMPLEMENTED

// CB declaration
// renderFunc and idleFunc are called from the render loop; the rest are
// registered in GLFW, hence their signatures
void renderFunc();
void idleFunc();
void resizeFunc(GLFWwindow *w, int width, int height);
void keyboardFunc(GLFWwindow *w, unsigned int codepoint);
void mouseFunc(GLFWwindow *w, int button, int action, int mods);

// Initialization and destruction functions
void initContext(int argc, char **argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initObj();
void destroy();

// Load the specified shader & return shader ID
// TO BE IMPLEMENTED
GLuint loadShader(const char *fileName, GLenum type);

// Texture creation, configuration and upload to OpenGL
// Return texture ID
// TO BE IMPLEMENTED
unsigned int loadTex(const char *fileName);

int main(int argc, char **argv)
{
#ifdef _WIN32
	std::locale::global(std::locale("spanish")); // Spanish accents
#endif

	initContext(argc, argv);
	initOGL();
	std::string vertexShader = std::string(SHADERS_PATH) + "/phong.vert";
	std::string fragmentShader = std::string(SHADERS_PATH) + "/phong.frag";
	initShader(vertexShader.c_str(), fragmentShader.c_str());
	initObj();

	destroy();

	return 0;
}

//////////////////////////////////////////
// Auxiliar functions
void initContext(int argc, char **argv) {}
void initOGL() {}
void destroy() {}
void initShader(const char *vname, const char *fname) {}
void initObj() {}

GLuint loadShader(const char *fileName, GLenum type) { return 0; }
unsigned int loadTex(const char *fileName) { return 0; }

void renderFunc() {}
void idleFunc() {}
void resizeFunc(GLFWwindow *w, int width, int height) {}
void keyboardFunc(GLFWwindow *w, unsigned int codepoint) {}
void mouseFunc(GLFWwindow *w, int button, int action, int mods) {}
