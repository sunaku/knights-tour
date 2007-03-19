// CMPE-220, Winter 2007, final project
// Knight's Tour in Cg
// Suraj Kurapati

#include <GL/glew.h>

// checks for required extensions and exits if they are not present
// this must be called after GLUT has already been set up.
void glew_init() {
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }

  if (!GLEW_EXT_framebuffer_object) {
    fprintf(stderr, "Error: EXT_framebuffer_object is not present.\n");
    exit(EXIT_FAILURE);
  }

  if (!GLEW_ARB_texture_rectangle) {
    fprintf(stderr, "Error: ARB_texture_rectangle is not present.\n");
    exit(EXIT_FAILURE);
  }

  int i;
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &i);
  if (i < 1) {
    fprintf(stderr, "Error: GL_MAX_COLOR_ATTACHMENTS_EXT is %d. Need at least 1.\n");
    exit(EXIT_FAILURE);
  }
}

void glew_fini() {}
