#version 130

in vec4 vPosition;
in vec4 vColor;

out vec4 vs_fs_color;

void
main()
{
  gl_Position = vPosition;
  vs_fs_color = vColor;
}
