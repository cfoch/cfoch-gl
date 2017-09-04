#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "LoadShaders.h"

#define BUFFER_OFFSET(x)   ((const void *) x)

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ColorBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

using namespace std;

void
init(void)
{
  GLuint program;
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);

  GLfloat vertices[NumVertices][2] = {
    { -0.90, -0.90 },
    {  0.85, -0.90 },
    { -0.90,  0.85 },
    {  0.90, -0.85 },
    {  0.90,  0.90 },
    { -0.85,  0.90 }
  };

  GLfloat colors[NumVertices][4] = {
    { 1.00,  0.00, 0.00, 1.00 },
    { 1.00,  0.00, 0.00, 1.00 },
    { 1.00,  0.00, 0.00, 1.00 },
    { 0.00,  0.00, 1.00, 1.00 },
    { 0.00,  0.00, 1.00, 1.00 },
    { 0.00,  0.00, 1.00, 1.00 }
  };

  glGenBuffers(NumBuffers, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(NumBuffers, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  ShaderInfo shaders[] = {
    { GL_VERTEX_SHADER, "triangle.vert" },
    { GL_FRAGMENT_SHADER, "triangle.frag" },
    { GL_NONE, NULL }
  };

  program = LoadShaders(shaders);
  glUseProgram(program);

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vColor);
  
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glFlush();
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitContextVersion(3, 0);
  glutInitWindowSize(480, 640);

  glutCreateWindow(argv[0]);
  if (glewInit()) {
    cerr << "Error initializing GLEW." << endl;
    exit(EXIT_FAILURE);
  }
  init();
  glutDisplayFunc(display);
  glutMainLoop();
}
