#include <GL/glut.h>

// GPGPU concept 3b: Use orthographic projection.
// This code sets the projection matrix to orthographic with a range of [-1,1] in the X and Y
// dimensions. This allows a trivial mapping of pixels to texels.
void glut_reshape_handler(int w, int h)
{
  if (h == 0) h = 1;
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void glut_idle_handler(void) {
  glutPostRedisplay();
}

void glut_init(int argc, char **argv, const char* const aWindowTitle, const unsigned aWindowWidth, const unsigned aWindowHeight) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(aWindowWidth, aWindowHeight);
  glutCreateWindow(aWindowTitle);

  glutIdleFunc(glut_idle_handler);
  glutReshapeFunc(glut_reshape_handler);
}
