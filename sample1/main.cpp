#include <iostream>
#include "LoadShaders.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

using namespace std;

enum VAO_IDs {Triangles, NumVAOs};
enum Buffer_IDs {ArrayBuffer, NumBuffers};
enum Attrib_IDs {vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 8;

void
init(void)
{
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);

  GLfloat vertices[NumVertices][2] = {
    {  0.00,  0.00 },
    {  0.50,  0.00 },
    {  0.50,  0.25 },
    {  0.25,  0.50 },
    {  0.00,  0.75 },
    { -0.25,  0.50 },
    { -0.50,  0.25 },
    { -0.50,  0.00 }
  };

  glGenBuffers(NumBuffers, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  ShaderInfo shaders[] = {
    {GL_VERTEX_SHADER, "triangles.vert"},
    {GL_FRAGMENT_SHADER, "triangles.frag"},
    {GL_NONE, NULL}
  };

  GLuint program = LoadShaders(shaders);
  glUseProgram(program);

  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

void
display(void)
{
  glClearColor(0.37, 0.59, 0.30, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);
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
