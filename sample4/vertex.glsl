#version 130

in vec4 vPosition;
uniform mat4 mvp;

void
main()
{
  gl_Position = mvp * vPosition;
}
