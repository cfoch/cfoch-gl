#ifndef LOAD_SHADERS__
#define LOAD_SHADERS__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

typedef struct {
  GLenum type;
  const char *filename;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo *shaders);

#endif
