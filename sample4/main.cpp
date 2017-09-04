#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LoadShaders.h"

#define BUFFER_OFFSET(x) ((const void *) (x))

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_VERTICES 100000
#define MAX_TRIANGLES 100000

struct _Surface {
  GLuint vao;
  GLuint vbo;
  GLuint ibo;

  GLfloat vertices[MAX_VERTICES][3];
  GLushort triangles[MAX_TRIANGLES][3];
  GLuint n_vertices;
  GLuint n_triangles;

  GLint attr_position;
};
typedef struct _Surface TSurface;


TSurface *surface1;
GLint uniform_mvp;

using namespace std;

TSurface *
t_surface_from_off_file(const char *filename)
{
  TSurface *surface;
  FILE *f;
  int i, dummy;
  char buffer[128];

  f = fopen(filename, "r");
  surface = new TSurface;

  fscanf(f, "%s", &buffer);
  fscanf(f, "%d %d %d", &surface->n_vertices, &surface->n_triangles, &dummy);

  for (i = 0; i < surface->n_vertices; i++) {
    fscanf(f, "%f %f %f", &surface->vertices[i][0], &surface->vertices[i][1],
        &surface->vertices[i][2]);
  }
  for (i = 0; i < surface->n_triangles; i++) {
    fscanf(f, "%d %d %d %d", &dummy, &surface->triangles[i][0],
        &surface->triangles[i][1], &surface->triangles[i][2]);
    //printf("%d %d %d\n", surface->triangles[i][0], surface->triangles[i][1],
    //    surface->triangles[i][2]);
  }
  fclose(f);
  return surface;
}

void
t_surface_init(TSurface *surface)
{
  glGenVertexArrays(1, &surface->vao);
  glBindVertexArray(surface->vao);

  glGenBuffers(1, &surface->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, surface->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * surface->n_vertices,
      surface->vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &surface->ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      sizeof(GLushort) * 3 * surface->n_triangles, surface->triangles,
      GL_STATIC_DRAW);
}

void
init(void)
{
  GLuint program;
  ShaderInfo shaders[] = {
    { GL_VERTEX_SHADER, "vertex.glsl" },
    { GL_FRAGMENT_SHADER, "fragment.glsl" },
    { GL_NONE, NULL }
  };

  surface1 = t_surface_from_off_file("NR0.off");

  program = LoadShaders(shaders);
  cout << "program " << program << endl;
  glUseProgram(program);


  if ((surface1->attr_position = glGetAttribLocation(program,
        "vPosition")) == -1) {
    cout << "Not possible to set vPosition." << endl;
    exit(1);
  }

  if ((uniform_mvp = glGetUniformLocation(program, "mvp")) == -1) {
    cout << "Not possible to set mvp." << endl;
    exit(1);
  }

  glEnableVertexAttribArray(surface1->attr_position);
  glBindBuffer(GL_ARRAY_BUFFER, surface1->vbo);
  glVertexAttribPointer(surface1->attr_position, 3, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(0));
}

void
display(void)
{
  glm::mat4 model = glm::translate(glm::mat4(1.0f),
      glm::vec3(0.0f, 0.0f, -1.0f));
  glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 projection = glm::perspective(45.0f,
      1.0f * SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 10.0f);
  glm::mat4 mvp = projection * view * model;

  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

  glClear(GL_COLOR_BUFFER_BIT);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface1->ibo);
  glDrawElements(GL_TRIANGLES, surface1->n_triangles * 3, GL_UNSIGNED_SHORT, 0);

  glFlush();
}

void
reshape(int w, int h)
{
  glViewport(0, 0, w, h);
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 0);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

  glutCreateWindow(argv[0]);

  if (glewInit()) {
    cerr << "GLEW could not be initialized." << endl;
    exit(EXIT_FAILURE);
  }

  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutMainLoop();

  return 0;
}
