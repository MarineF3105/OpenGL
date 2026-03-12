#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<glm/glm.hpp>
#include<cmath>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
using namespace std;

string LoadFile(const char* path)
{
 ifstream file(path);
 stringstream buf;
 buf << file.rdbuf();
 return buf.str();
}
string vertSrc = LoadFile("shader/shader.vert");
string fragSrc = LoadFile("shader/shader.frag");

const char* v = vertSrc.c_str();
const char* f = fragSrc.c_str();

GLuint shaderProgram;
int uniformXMove;
int uniformRGB;

bool direction = false;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

struct Engine
{
 GLuint WIDTH = 800;
 GLuint HEIGHT = 600;
 int bufferWidth, bufferHeight;
 GLFWwindow* window;
 Engine()
 {
  
  if (!glfwInit())
  {
   cout << "Failed to Initialize GLFW\n";
   glfwTerminate();
   exit(EXIT_FAILURE);
  } 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  window = glfwCreateWindow(WIDTH, HEIGHT, "TEST", NULL, NULL);
  if (!window)
  {
   cout << "Window Could'nt be Created\n";
   glfwTerminate();
   exit(EXIT_FAILURE);
  }
  glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
  glfwMakeContextCurrent(window);
glewExperimental = GL_TRUE;
GLenum err = glewInit();

if (err != GLEW_OK) {
    printf("GLEW init returned error 0x%x (%s) — ignoring and continuing...\n",
           err, glewGetErrorString(err));
    // IMPORTANT: Do NOT exit/return here — proceed to rendering
} else {
    printf("GLEW initialized OK.\n");
}

  glViewport(0, 0, bufferWidth, bufferHeight);
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &v, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &f, NULL);
  glCompileShader(fragmentShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  int success;
  char infoLog[512];

  // check vertex shader
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
   glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
   cout << "VERTEX SHADER ERROR:\n" << infoLog << endl;
  }

  // check fragment shader
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
   glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
   cout << "FRAGMENT SHADER ERROR:\n" << infoLog << endl;
  }

  // check program link
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
   glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
   cout << "PROGRAM LINK ERROR:\n" << infoLog << endl;
  }

 }
};  
Engine engine;
struct Triangle
{
 GLuint VAO, VBO;
 Triangle()
 {
  float vertices[] =
  {
   0.0,0.4,0.0,
   -0.4,-0.4,0.0,
   0.4,-0.4,0.0
  };
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
 }


 void draw(GLuint shaderProgram)
 {
  glUseProgram(shaderProgram);
  uniformXMove = glGetUniformLocation(shaderProgram, "xMove");
  uniformRGB = glGetUniformLocation(shaderProgram, "rgb");

  float r = (triOffset + 1.0f) * 1.0f;
  float g = abs(sin(triOffset * 5));
  float b = 1.0f - r ;

  glUniform3f(uniformRGB, r, g, b);

  glUniform1f(uniformXMove, triOffset);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  
 }
};
Triangle triangle;
int main()
{
 while (!glfwWindowShouldClose(engine.window))
 {
  glfwPollEvents();
  
  if (direction)
  {
   triOffset += triIncrement;

  }
  else
  {
   triOffset -= triIncrement;
  }

  if (abs(triOffset) >= triMaxOffset)
  {
   direction = !direction;
  }

  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  triangle.draw(shaderProgram);
  glfwSwapBuffers(engine.window);



 }
 return 0;
}
