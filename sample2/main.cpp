#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "LoadShaders.h"

#define BUFFER_OFFSET(x) ((const void*) (x))
#define USE_PRIMITIVE_RESTART 0

using namespace std;

GLuint program;
GLuint vao[1];
GLuint ebo[1];
GLuint vbo[1];

// GLint render_model_matrix_loc;
// GLint render_projection_matrix_loc;
GLint render_view_matrix;
GLint render_rotation_matrix;

GLfloat angle = 0;

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

  // render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
  // render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");
  render_view_matrix = glGetUniformLocation(program, "view_matrix");
  render_rotation_matrix = glGetUniformLocation(program, "rotation_matrix");

  static GLfloat cube_positions[] = {
    -0.3f, -0.3f, -0.3f, 1.0f,
    -0.3f, -0.3f,  0.3f, 1.0f,
    -0.3f,  0.3f, -0.3f, 1.0f,
    -0.3f,  0.3f,  0.3f, 1.0f,
     0.3f, -0.3f, -0.3f, 1.0f,
     0.3f, -0.3f,  0.3f, 1.0f,
     0.3f,  0.3f, -0.3f, 1.0f,
     0.3f,  0.3f,  0.3f, 1.0f
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

  // Set up the model and projection matrix
  // mat4 model_matrix(translate(0.0f, 0.0f, -5.0f) * rotate(t * 360.0f, Y) * rotate(t * 720.0f, Z));
  // vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 500.0f));

  // glUniformMatrix4fv(render_model_matrix_loc, 4, GL_FALSE, model_matrix);
  // glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

  glm::mat4 view_matrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(render_view_matrix, 1, GL_FALSE, &view_matrix[0][0]);

  glm::mat4 rotation_matrix = glm::rotate(angle, glm::vec3(1.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(render_rotation_matrix, 1, GL_FALSE, &rotation_matrix[0][0]);

  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

  #if USE_PRIMITIVE_RESTART
  printf("Primitive restart\n");
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFF);
  glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
  #else
  printf("NO Primitive restart\n");
  glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
  glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT,
      (const GLvoid *) (9 *sizeof(GLushort)));
  #endif
  glFlush();

  if (angle >= 0.0f)
    angle += 0.001f;
  else if (angle <= 360.0f)
    angle -= 0.001f;
  glutPostRedisplay();
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
