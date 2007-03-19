// CMPE-220, Winter 2007, final project
// Knight's Tour in Cg
// Suraj Kurapati

#include <GL/glu.h>

GLuint fbo__id;

void fbo_init(const float aTexWidth, const float aTexHeight) {
  // create FBO and bind it (that is, use offscreen render target)
    glGenFramebuffersEXT(1, &fbo__id);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo__id);

  // set viewport for 1:1 pixel to texel mapping
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, aTexWidth, 0, aTexHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, aTexWidth, aTexHeight);
}

void fbo_fini() {
  glDeleteFramebuffersEXT(1, &fbo__id);
}

void fbo_enable() {
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo__id);
}

void fbo_disable() {
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void fbo_check() {
  switch(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
  {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      printf("Framebuffer incomplete, incomplete attachment\n");
      exit(EXIT_FAILURE);

    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      printf("Unsupported framebuffer format\n");
      exit(EXIT_FAILURE);

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      printf("Framebuffer incomplete, missing attachment\n");
      exit(EXIT_FAILURE);

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      printf("Framebuffer incomplete, attached images must have "
      "same dimensions\n");
      exit(EXIT_FAILURE);

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      printf("Framebuffer incomplete, attached images must have "
      "same format\n");
      exit(EXIT_FAILURE);

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      printf("Framebuffer incomplete, missing draw buffer\n");
      exit(EXIT_FAILURE);

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      printf("Framebuffer incomplete, missing read buffer\n");
      exit(EXIT_FAILURE);

    default:
      printf("*** Unknown framebuffer status! ***\n");
      exit(EXIT_FAILURE);
  }
}
