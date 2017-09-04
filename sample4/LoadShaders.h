#ifndef __LOAD_SHADERS__
#define __LOAD_SHADERS__

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "string.h"

struct ShaderInfo {
  GLuint type;
  const char *filename;
};

GLuint LoadShaders(ShaderInfo *shaders);

#endif /* __LOAD_SHADERS__ */
