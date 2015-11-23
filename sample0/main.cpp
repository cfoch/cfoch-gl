#include <GL/freeglut.h>
#include <iostream>

using namespace std;

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutCreateWindow("test");
  cout << glGetString(GL_VERSION) << endl;
  cout << glGetString(GL_VENDOR) << endl;
  cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

  return 0;
}
