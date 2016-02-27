#include <iostream>
#include "LoadShaders.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

using namespace std;

enum VAO_IDs {TrianglesUp, TrianglesDown, NumVAOs};
enum Buffer_IDs {ArrayUpBuffer, ArrayDownBuffer, NumBuffers};
enum Attrib_IDs {vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVerticesUp = 8;
const GLuint NumVerticesDown = 3;

void
init(void)
{
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[TrianglesUp]);

  GLfloat verticesUp[NumVerticesUp][3] = {
    {  0.00,  0.00, 0.0 },
    {  0.50,  0.00, 0.0 },
    {  0.50,  0.25, 0.0 },
    {  0.25,  0.50, 0.0 },
    {  0.00,  0.75, 0.0 },
    { -0.25,  0.50, 0.0 },
    { -0.50,  0.25, 0.0 },
    { -0.50,  0.00, 0.0 }
  };

  glGenBuffers(NumBuffers, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayUpBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesUp), verticesUp, GL_STATIC_DRAW);


  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);

  glBindVertexArray(VAOs[TrianglesDown]);

  GLfloat verticesDown[NumVerticesDown][3] = {
    {  0.00,  0.00, 0.0 },
    { -0.50,  0.00, 0.0 },
    {  0.00, -0.50, 0.0 }
  };

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayDownBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesDown), verticesDown,
      GL_STATIC_DRAW);

  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

void
display(void)
{
  GLuint program;

  glClearColor(0.37, 0.59, 0.30, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);


  // UP
  glBindVertexArray(VAOs[TrianglesUp]);
  ShaderInfo shadersUp[] = {
    {GL_VERTEX_SHADER, "triangles.vert"},
    {GL_FRAGMENT_SHADER, "trianglesUp.frag"},
    {GL_NONE, NULL}
  };
  program = LoadShaders(shadersUp);
  glUseProgram(program);

  glDrawArrays(GL_TRIANGLE_FAN, 0, NumVerticesUp);

  // DOWN
  glBindVertexArray(VAOs[TrianglesDown]);
  ShaderInfo shadersDown[] = {
    {GL_VERTEX_SHADER, "triangles.vert"},
    {GL_FRAGMENT_SHADER, "trianglesDown.frag"},
    {GL_NONE, NULL}
  };
  program = LoadShaders(shadersDown);
  glUseProgram(program);

  glDrawArrays(GL_TRIANGLES, 0, NumVerticesDown);

  glFlush();
}

int
main(int argc, char ** argv)
{
  const GLubyte *version;  

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(512, 512);
  glutInitContextVersion(3, 0);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutCreateWindow(argv[0]);
  if (glewInit()) {
    cerr << "Unable to initialize GLEW. Exiting" << endl;
    exit(EXIT_FAILURE);
  }
  init();
  glutDisplayFunc(display);
  glutMainLoop();
}
