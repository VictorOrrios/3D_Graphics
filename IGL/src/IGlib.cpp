#include <IGL/IGlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <fstream>

#include "OGLObject.h"

namespace IGlib
{
	//////////////////////////////////////////////////////////////
	// Global variables
	//////////////////////////////////////////////////////////////

	// Window
	GLFWwindow *window = NULL;

	// Matrices
	glm::mat4 proj = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	// Background color
	glm::vec4 bgColor(0.0f);

	// Shaders
	unsigned int vshader;
	unsigned int fshader;
	unsigned int program;

	// Uniform matrix ID variables
	int uProjMat;
	int uModelViewMat;
	int uModelMat;
	int uViewMat;
	int uModelViewProjMat;
	int uNormalMat;

	// Uniform texture ID variables
	int uColorTex;
	int uEmiTex;
	int uNormalTex;
	int uSpecularTex;
	int uAuxiliarTex;

	// Attributes
	int inPos;
	int inColor;
	int inNormal;
	int inTangent;
	int inTexCoord;

	// Collection of objects to be drawn
	std::vector<OGLObject *> oGLObjectList;

	// CBs
	tResizeCB ResizeCB = NULL;
	tIdleCB IdleCB = NULL;
	tKeyboardCB KeyboardCB = NULL;
	tMouseCB MouseCB = NULL;
	tMouseMotionCB MouseMotionCB = NULL;

	// Last known cursor position, in pixels.
	// GLFW reports it separately from the keyboard and button events, but our
	// callbacks expose it the way GLUT did.
	int cursorX = 0;
	int cursorY = 0;

	// How many mouse buttons are currently held down. GLUT only reported motion
	// while dragging, so we reproduce that behaviour.
	int buttonsDown = 0;

	////////////////////////////////////////
	// Auxiliar functions declaration
	////////////////////////////////////////
	void shaderInit(const char *vname, const char *fname);
	void render();

	// GLFW CB
	void framebufferSizeEvent(GLFWwindow *w, int width, int height);
	void charEvent(GLFWwindow *w, unsigned int codepoint);
	void mouseButtonEvent(GLFWwindow *w, int button, int action, int mods);
	void cursorPosEvent(GLFWwindow *w, double x, double y);
	void errorEvent(int error, const char *description);
	void GLAD_API_PTR debugEvent(GLenum source, GLenum type, GLuint id, GLenum severity,
								 GLsizei length, const GLchar *message, const void *userParam);

	////////////////////////////////////////
	// Library functions
	////////////////////////////////////////
	bool init(const char *vertexShaderFileName, const char *fragmentShaderFileName)
	{
		// GLFW & context
		glfwSetErrorCallback(errorEvent);

		if (!glfwInit())
		{
			std::cout << "Error: could not initialize GLFW" << std::endl;
			return false;
		}

		// We ask for 4.6 and fall back to older versions, so that the exercises
		// also run on machines whose driver does not reach the latest one.
		// 4.3 is the last version with debug output, 3.3 is the minimum required.
		const int requestedVersions[][2] = {{4, 6}, {4, 3}, {3, 3}};
		const int nRequestedVersions = 3;

		for (int i = 0; i < nRequestedVersions && window == NULL; i++)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, requestedVersions[i][0]);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, requestedVersions[i][1]);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

			// Framebuffer & Window
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
			glfwWindowHint(GLFW_DEPTH_BITS, 24);

			window = glfwCreateWindow(500, 500, "G3D Exercises", NULL, NULL);
		}

		if (!window)
		{
			std::cout << "Error: could not create the window. "
					  << "This system does not seem to support OpenGL 3.3 core." << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		// Extensions
		// Wait for OpenGL context creation
		int version = gladLoadGL(glfwGetProcAddress);
		if (version == 0)
		{
			std::cout << "Error: could not load the OpenGL functions" << std::endl;
			glfwDestroyWindow(window);
			glfwTerminate();
			return false;
		}

		const GLubyte *oglVersion = glGetString(GL_VERSION);
		std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

		// Debug output: turns silent OpenGL errors into readable messages.
		// Only available from 4.3 onwards, hence the guard.
		int flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (GLAD_GL_VERSION_4_3 && (flags & GL_CONTEXT_FLAG_DEBUG_BIT))
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(debugEvent, NULL);
			// Everything but the notification spam
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
								  0, NULL, GL_FALSE);
		}

		// Callbacks
		glfwSetFramebufferSizeCallback(window, framebufferSizeEvent);
		glfwSetCharCallback(window, charEvent);
		glfwSetMouseButtonCallback(window, mouseButtonEvent);
		glfwSetCursorPosCallback(window, cursorPosEvent);

		// OpenGL
		// Pipeline configuration
		glEnable(GL_DEPTH_TEST);
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);

		proj = glm::perspective(float(glm::radians(45.0f)), 1.0f, 0.1f, 50.0f);
		view = glm::mat4(1.0f);

		// Shaders
		shaderInit(vertexShaderFileName, fragmentShaderFileName);

		return true;
	}

	void destroy()
	{
		for (unsigned int i = 0; i < oGLObjectList.size(); i++)
			delete oGLObjectList[i];

		oGLObjectList.clear();

		glDetachShader(program, vshader);
		glDetachShader(program, fshader);
		glDeleteShader(vshader);
		glDeleteShader(fshader);
		glDeleteProgram(program);

		if (window != NULL)
		{
			glfwDestroyWindow(window);
			window = NULL;
		}
		glfwTerminate();
	}

	void mainLoop()
	{
		// GLFW has no glutMainLoop: the render loop is explicit.
		// GLUT called the idle CB when no events were pending; here it runs once
		// per frame, which is what the exercises expect for animation.
		while (!glfwWindowShouldClose(window))
		{
			if (IdleCB != NULL)
				IdleCB();

			render();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void setProjMat(const glm::mat4 &projMat)
	{
		proj = projMat;
	}

	void setViewMat(const glm::mat4 &viewMat)
	{
		view = viewMat;
	}

	int createObj(unsigned int nTriangles, unsigned int nVertex,
				  const unsigned int *triangleIdx, const float *vertexPos,
				  const float *vertexColor, const float *vertexNormal,
				  const float *vertexTexCoord, const float *vertexTangent)
	{
		OGLObject *obj = new OGLObject;
		oGLObjectList.push_back(obj);
		obj->create(nTriangles, nVertex, triangleIdx,
					vertexPos, vertexColor, vertexNormal, vertexTexCoord, vertexTangent);

		return int(oGLObjectList.size()) - 1;
	}

	void setModelMat(int ID, const glm::mat4 &modelMat)
	{
		oGLObjectList[ID]->model = modelMat;
	}

	void addColorTex(int ID, const char *fileName)
	{
		oGLObjectList[ID]->loadColorTexture(fileName);
	}

	void addEmissiveTex(int ID, const char *fileName)
	{
		oGLObjectList[ID]->loadEmissiveTexture(fileName);
	}

	void addNormalTex(int ID, const char *fileName)
	{
		oGLObjectList[ID]->loadNormalTexture(fileName);
	}

	void addSpecularTex(int ID, const char *fileName)
	{
		oGLObjectList[ID]->loadSpecularTexture(fileName);
	}

	void addAuxiliarTex(int ID, const char *fileName)
	{
		oGLObjectList[ID]->loadAuxiliarTexture(fileName);
	}

	void setResizeCB(tResizeCB resizeCB)
	{
		ResizeCB = resizeCB;
	}

	void setIdleCB(tIdleCB idleCB)
	{
		IdleCB = idleCB;
	}

	void setKeyboardCB(tKeyboardCB keyboardCB)
	{
		KeyboardCB = keyboardCB;
	}

	void setMouseCB(tMouseCB mouseCB)
	{
		MouseCB = mouseCB;
	}

	void setMouseMoveCB(tMouseMotionCB mouseMotionCB)
	{
		MouseMotionCB = mouseMotionCB;
	}

	//////////////////////////////////////////////////////////////
	// Auxiliar functions
	//////////////////////////////////////////////////////////////

	// GLFW CB
	void errorEvent(int error, const char *description)
	{
		std::cout << "GLFW error " << error << ": " << description << std::endl;
	}

	void GLAD_API_PTR debugEvent(GLenum source, GLenum type, GLuint id, GLenum severity,
								 GLsizei length, const GLchar *message, const void *userParam)
	{
		std::cout << "OpenGL debug [" << id << "]: " << message << std::endl;
	}

	void framebufferSizeEvent(GLFWwindow *w, int width, int height)
	{
		glViewport(0, 0, width, height);
		if (ResizeCB != NULL)
			ResizeCB(width, height);
	}

	void charEvent(GLFWwindow *w, unsigned int codepoint)
	{
		// Only the printable characters GLUT used to report through glutKeyboardFunc
		if (codepoint > 255)
			return;

		if (KeyboardCB != NULL)
			KeyboardCB((unsigned char)codepoint, cursorX, cursorY);
	}

	void mouseButtonEvent(GLFWwindow *w, int button, int action, int mods)
	{
		int b = 0, s = 0;
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			b = 0;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			b = 1;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			b = 2;
			break;
		default:
			b = 3;
			break;
		}

		if (action == GLFW_PRESS)
		{
			s = 0;
			buttonsDown++;
		}
		if (action == GLFW_RELEASE)
		{
			s = 1;
			if (buttonsDown > 0)
				buttonsDown--;
		}

		if (MouseCB != NULL)
			MouseCB(b, s, cursorX, cursorY);
	}

	void cursorPosEvent(GLFWwindow *w, double x, double y)
	{
		cursorX = int(x);
		cursorY = int(y);

		// Like glutMotionFunc: only report movement while a button is held down
		if (buttonsDown > 0 && MouseMotionCB != NULL)
			MouseMotionCB(cursorX, cursorY);
	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		if (uProjMat != -1)
			glUniformMatrix4fv(uProjMat, 1, GL_FALSE, &(proj[0][0]));
		if (uViewMat != -1)
			glUniformMatrix4fv(uViewMat, 1, GL_FALSE, &(view[0][0]));

		for (unsigned int i = 0; i < oGLObjectList.size(); i++)
			oGLObjectList[i]->render();

		glUseProgram(0);
	}

	// Load shader functions
	char *loadStringFromFile(const char *fileName, unsigned int &fileLen)
	{
		// File loading
		std::ifstream file;
		file.open(fileName, std::ios::in);
		if (!file)
			return 0;

		// Compute file length
		file.seekg(0, std::ios::end);
		fileLen = (unsigned int)file.tellg();
		file.seekg(std::ios::beg);

		// Read the file
		char *source = new char[fileLen + 1];

		int i = 0;
		while (file.good())
		{
			source[i] = char(file.get());
			if (!file.eof())
				i++;
			else
				fileLen = i;
		}
		source[fileLen] = '\0';
		file.close();

		return source;
	}

	GLuint loadShader(const char *fileName, GLenum type)
	{
		unsigned int fileLen;
		char *source = loadStringFromFile(fileName, fileLen);

		if (source == 0)
		{
			std::cout << "Error loading file: " << fileName << std::endl;
			return 0;
		}

		//////////////////////////////////////////////
		// Shader creation & compilation
		GLuint shader;
		shader = glCreateShader(type);
		glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
		glCompileShader(shader);
		delete[] source;

		// Check compilation status
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			// Compute error string
			GLint logLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

			char *logString = new char[logLen];
			glGetShaderInfoLog(shader, logLen, NULL, logString);
			std::cout << "Errors in " << fileName << std::endl;
			std::cout << logString << std::endl;
			delete[] logString;

			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	void shaderInit(const char *vname, const char *fname)
	{
		// Compilation
		vshader = loadShader(vname, GL_VERTEX_SHADER);
		fshader = loadShader(fname, GL_FRAGMENT_SHADER);

		// Link
		program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);

		glBindAttribLocation(program, 0, "inPos");
		glBindAttribLocation(program, 1, "inColor");
		glBindAttribLocation(program, 2, "inNormal");
		glBindAttribLocation(program, 3, "inTexCoord");
		glBindAttribLocation(program, 4, "inTangent");

		glLinkProgram(program);

		int linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			// Compute error string
			GLint logLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

			char *logString = new char[logLen];
			glGetProgramInfoLog(program, logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete[] logString;

			glDeleteProgram(program);
			program = 0;
			return;
		}

		// Uniforms
		uProjMat = glGetUniformLocation(program, "proj");
		uViewMat = glGetUniformLocation(program, "view");
		uModelMat = glGetUniformLocation(program, "model");
		uNormalMat = glGetUniformLocation(program, "normal");
		uModelViewMat = glGetUniformLocation(program, "modelView");
		uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

		uColorTex = glGetUniformLocation(program, "colorTex");
		uEmiTex = glGetUniformLocation(program, "emiTex");
		uNormalTex = glGetUniformLocation(program, "normalTex");
		uSpecularTex = glGetUniformLocation(program, "specularTex");
		uAuxiliarTex = glGetUniformLocation(program, "auxiliarTex");

		// Attributes
		inPos = glGetAttribLocation(program, "inPos");
		inColor = glGetAttribLocation(program, "inColor");
		inNormal = glGetAttribLocation(program, "inNormal");
		inTexCoord = glGetAttribLocation(program, "inTexCoord");
		inTangent = glGetAttribLocation(program, "inTangent");
	}

}
