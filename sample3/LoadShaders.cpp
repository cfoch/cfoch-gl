#include "LoadShaders.h"

using namespace std;

static const GLchar *
read_shader(const char *filename)
{
  string shader;
  ifstream f(filename, ios::in);
  if (!f) {
    cerr << "File " << filename << "could not be read." << endl;
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
  GLint status;
  int i;
  program = glCreateProgram();
  for (i = 0; shaders[i].type != GL_NONE; i++) {
    const GLchar *shader_text;
    GLuint shader;
    shader = glCreateShader(shaders[i].type);
    shader_text = read_shader(shaders[i].filename);

    cout << shader_text << endl;

    glShaderSource(shader, 1, &shader_text, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
      char log[2048];
      glGetShaderInfoLog(shader, 2048, NULL, log);
      cerr << log << endl;
      exit(1);
    }
    glAttachShader(program, shader);    
  }
  glLinkProgram(program);

  return program;
}
