#include <iostream>
#include "LoadShaders.h"

#define BUFFER_OFFSET(x) ((const void*) (x))
#define USE_PRIMITIVE_RESTART 1

using namespace std;

GLuint program;
GLuint vao[1];
GLuint ebo[1];
GLuint vbo[1];

void
init(void)
{
  const GLchar *names[3] = {"position", "color", NULL};

  ShaderInfo shader_info[] = {
    {GL_VERTEX_SHADER, "cube.vert"},
    {GL_FRAGMENT_SHADER, "cube.frag"},
    {GL_NONE, NULL}
  };

  program = LoadShaders(shader_info, names);
  glUseProgram(program);

  static GLfloat cube_positions[] = {
    -1.0f, -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f
  };

  static const GLfloat cube_colors[] = {
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f
  };

  static GLushort cube_indices[] = {
    0, 1, 2, 3, 6, 7, 4, 5,
    0xFFFF,
    2, 6, 0, 4, 1, 5, 3, 7
  };
  glGenBuffers(1, ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices,
      GL_STATIC_DRAW);

  // Set up the vertex attributes

  glGenVertexArrays(1, vao);
  glBindVertexArray(vao[0]);

  glGenBuffers(1,vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions) + sizeof(cube_colors),
      NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors),
      cube_colors);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
      (const GLvoid *) sizeof(cube_positions));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void
display(void)
{
  // GLuint program;
  glClearColor(0.37, 0.59, 0.30, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

  #if USE_PRIMITIVE_RESTART
  printf("Primitive restart\n");
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFF);
  printf("hello1\n");
  glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
  #else
  printf("NO Primitive restart\n");
  glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
  glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT,
      (const GLvoid *) (9 *sizeof(GLushort)));
  #endif
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
