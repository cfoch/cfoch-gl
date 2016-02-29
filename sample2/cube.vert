#version 130

uniform mat4 view_matrix;

in vec4 position;
in vec4 color;

out vec4 vs_fs_color;

void
main()
{
  vs_fs_color = color;
  gl_Position = position * view_matrix;
}
