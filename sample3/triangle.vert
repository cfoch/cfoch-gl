#version 130

uniform float delta_x;
uniform float delta_y;
uniform mat4 rotation_matrix;

in vec4 vPosition;
in vec4 vColor;

out vec4 vs_fs_color;

void
main()
{
  gl_Position = vPosition + vec4(delta_x, delta_y, 0.0, 0.0);
  gl_Position = rotation_matrix * gl_Position;
  vs_fs_color = vColor;
}
