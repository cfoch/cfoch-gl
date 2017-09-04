#include "LoadShaders.h"

using namespace std;

static const char *
read_shader(const char *filename)
{
  string shader;
  ifstream f(filename, ios::in);
  if (!f) {
    cerr << "File " << filename << " could not be read properly." << endl;
    exit(1);
  }
  stringstream buffer;
  buffer << f.rdbuf();
  shader = buffer.str();
  return strdup(shader.c_str());
}

GLuint
LoadShaders(ShaderInfo *shaders)
{
  GLuint program;
  int status, i;
  program = glCreateProgram();
  for (i = 0; shaders[i].type != GL_NONE; i++) {
    const GLchar *shader_text;
    GLuint shader = glCreateShader(shaders[i].type);
    shader_text = read_shader(shaders[i].filename);

    glShaderSource(shader, 1, &shader_text, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    cout << shader_text << endl;

    if (status != GL_TRUE) {
      char log[2048];
      glGetShaderInfoLog(shader, 2048, NULL, log);
      cerr << log << endl;
      exit(1);
    }
    glAttachShader(program, shader);
  }
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    cout << "Linking problem in shader." << endl;
    exit(1);
  }

  return program;
}
