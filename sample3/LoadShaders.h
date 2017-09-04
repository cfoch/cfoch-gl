#ifndef LOAD_SHADERS
#define LOAD_SHADERS

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <GL/glew.h>
#include <GL/freeglut.h>

struct ShaderInfo {
  GLuint type;
  const char *filename;
};

GLuint LoadShaders(ShaderInfo *shaders);

#endif /* LOAD_SHADERS */
