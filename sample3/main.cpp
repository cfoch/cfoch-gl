#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "LoadShaders.h"

#define BUFFER_OFFSET(x)   ((const void *) x)

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ColorBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

GLint uniform_delta_x;
GLint uniform_delta_y;
GLint uniform_rotation_matrix;

float delta_x = 0.0;
float delta_y = 0.0;
float delta_angle = 0.0;
const glm::vec3 rotation_axis(0.0, 0.0, 1.0);

using namespace std;

void
init(void)
{
  GLuint program;
  glClearColor(0.1, 0.2, 0.1, 1.0);
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);

  GLfloat vertices[NumVertices][2] = {
    { -0.30, -0.30 },
    {  0.25, -0.30 },
    { -0.30,  0.25 },
    {  0.30, -0.25 },
    {  0.30,  0.30 },
    { -0.25,  0.30 }
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

  /* Vertices */
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  /* Colors */
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
  glBindAttribLocation(program, vPosition, "vPosition");
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
  glBindAttribLocation(program, vColor, "vPosition");
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vColor);

  if ((uniform_delta_x = glGetUniformLocation(program, "delta_x")) == -1) {
    cout << "delta_x not specified." << endl;
    exit(EXIT_FAILURE);
  }
  if ((uniform_delta_y = glGetUniformLocation(program, "delta_y")) == -1) {
    cout << "delta_y not specified." << endl;
    exit(EXIT_FAILURE);
  }
  if ((uniform_rotation_matrix = glGetUniformLocation(program,
        "rotation_matrix")) == -1) {
    cout << "delta_y not specified." << endl;
    exit(EXIT_FAILURE);
  }
}

void
display(void)
{
  glm::mat4 rotation_matrix = glm::rotate(delta_angle, rotation_axis);

  glClear(GL_COLOR_BUFFER_BIT);
  glUniform1f(uniform_delta_x, delta_x);
  glUniform1f(uniform_delta_y, delta_y);
  glUniformMatrix4fv(uniform_rotation_matrix, 1, GL_FALSE,
      &rotation_matrix[0][0]);
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glFlush();
}

void
keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 'a':
      delta_x -= 0.05;
      break;
    case 'd':
      delta_x += 0.05;
      break;
    case 'w':
      delta_y += 0.05;
      break;
    case 's':
      delta_y -= 0.05;
      break;
    case 'q':
      delta_angle += 0.05;
      break;
    case 'e':
      delta_angle -= 0.05;
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitContextVersion(3, 0);
  glutInitWindowSize(480, 480);

  glutCreateWindow(argv[0]);
  if (glewInit()) {
    cerr << "Error initializing GLEW." << endl;
    exit(EXIT_FAILURE);
  }
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
}
