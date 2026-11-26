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
unsigned int vshader;
unsigned int fshader;
unsigned int program;

// VAO
unsigned int vao;

// VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;

// Variables Uniform
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;

// Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;

//////////////////////////////////////////////////////////////
// Auxiliar Functions
//////////////////////////////////////////////////////////////

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

int main(int argc, char **argv) {
#ifdef _WIN32
  std::locale::global(std::locale("spanish")); // Spanish accents
#endif

  initContext(argc, argv);
  initOGL();
  std::string vertexShader = std::string(SHADERS_PATH) + "/disney2012.vert";
  std::string fragmentShader = std::string(SHADERS_PATH) + "/disney2012.frag";
  initShader(vertexShader.c_str(), fragmentShader.c_str());
  initObj();

  while (!glfwWindowShouldClose(window)) {
    idleFunc();
    renderFunc();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  destroy();

  return 0;
}

//////////////////////////////////////////
// Auxiliar functions
void initContext(int argc, char **argv) {
  if (!glfwInit()) {
    std::cout << "Error: could not initialize GLFW" << std::endl;
    exit(-1);
  }

  const int requestedVersions[][2] = {{4, 6}, {4, 3}, {3, 3}};
  const int nRequestedVersions = 3;
  for (int i = 0; i < nRequestedVersions && window == NULL; i++) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, requestedVersions[i][0]);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, requestedVersions[i][1]);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    window = glfwCreateWindow(500, 500, "G3D Exercises", NULL, NULL);
  }
  if (!window) {
    std::cout << "Error: could not create the window" << std::endl;
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (gladLoadGL(glfwGetProcAddress) == 0) {
    std::cout << "Error: could not load the OpenGL functions" << std::endl;
    exit(-1);
  }
  const GLubyte *oglVersion = glGetString(GL_VERSION);
  std::cout << "This system supports OpenGL Version: " << oglVersion
            << std::endl;

  glfwSetFramebufferSizeCallback(window, resizeFunc);
  glfwSetCharCallback(window, keyboardFunc);
  glfwSetMouseButtonCallback(window, mouseFunc);
}
void initOGL() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

  glFrontFace(GL_CCW);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);

  proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
  view = glm::mat4(1.0f);
  view[3].z = -6;
}
void destroy() {
  glDetachShader(program, vshader);
  glDetachShader(program, fshader);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  glDeleteProgram(program);

  glfwDestroyWindow(window);
  glfwTerminate();
}
void initShader(const char *vname, const char *fname) {
  vshader = loadShader(vname, GL_VERTEX_SHADER);
  fshader = loadShader(fname, GL_FRAGMENT_SHADER);

  program = glCreateProgram();

  glAttachShader(program, vshader);
  glAttachShader(program, fshader);
  glLinkProgram(program);

  int linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    // Calculamos una cadena de error
    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    char *logString = new char[logLen];
    glGetProgramInfoLog(program, logLen, NULL, logString);
    std::cout << "Error: " << logString << std::endl;
    delete[] logString;
    glDeleteProgram(program);
    program = 0;
    exit(-1);
  }

  // Attributes
  glBindAttribLocation(program, 0, "inPos");
  glBindAttribLocation(program, 1, "inColor");
  glBindAttribLocation(program, 2, "inNormal");
  glBindAttribLocation(program, 3, "inTexCoord");
  inPos = glGetAttribLocation(program, "inPos");
  inColor = glGetAttribLocation(program, "inColor");
  inNormal = glGetAttribLocation(program, "inNormal");
  inTexCoord = glGetAttribLocation(program, "inTexCoord");

  // Uniforms
  uNormalMat = glGetUniformLocation(program, "normal");
  uModelViewMat = glGetUniformLocation(program, "modelView");
  uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
}
void initObj() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  if (inPos != -1) {
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
                 cubeVertexPos, GL_STATIC_DRAW);
    glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(inPos);
  }
  if (inColor != -1) {
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
                 cubeVertexColor, GL_STATIC_DRAW);
    glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(inColor);
  }
  if (inNormal != -1) {
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
                 cubeVertexNormal, GL_STATIC_DRAW);
    glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(inNormal);
  }
  if (inTexCoord != -1) {
    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2,
                 cubeVertexTexCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(inTexCoord);
  }
}

GLuint loadShader(const char *fileName, GLenum type) {
  unsigned int fileLen;
  char *source = loadStringFromFile(fileName, fileLen);
  //////////////////////////////////////////////
  // Creación y compilación del Shader
  GLuint shader;
  shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
  glCompileShader(shader);
  delete[] source;

  // Comprobamos que se compiló bien
  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    // Calculamos una cadena de error
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    char *logString = new char[logLen];
    glGetShaderInfoLog(shader, logLen, NULL, logString);
    std::cout << "Error: " << logString << std::endl;
    delete[] logString;
    glDeleteShader(shader);
    exit(-1);
  }

  return shader;
}
unsigned int loadTex(const char *fileName) { return 0; }

void renderFunc() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);
}
void idleFunc() {}
void resizeFunc(GLFWwindow *w, int width, int height) {}
void keyboardFunc(GLFWwindow *w, unsigned int codepoint) {}
void mouseFunc(GLFWwindow *w, int button, int action, int mods) {}
