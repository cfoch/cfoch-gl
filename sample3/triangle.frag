#version 130

out vec4 fColor;
in vec4 vs_fs_color;

void
main()
{
  fColor = vs_fs_color;
}
