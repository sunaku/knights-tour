#include "fbo.c"

GLuint board__width;
GLuint board__texture;
GLuint board__display;
float* board__data;

void board_init(const GLuint aWidth, const GLuint aKnightRow, const GLuint aKnightCol)
{
  board__width = aWidth;

  // generate the board data
    if((board__data = (float*)malloc(4*board__width*board__width*sizeof(float))) == NULL)
    {
      printf("\n*** out of memory allocating board__data ***\n");
      exit(2);
    }

    for (unsigned i = 0; i < board__width*board__width*4; i += 4)
    {
      board__data[i+0] = 0; //(rand() >= (RAND_MAX/4) ? 1.0 : 0.0); // set red
      board__data[i+1] = 0; // set green
      board__data[i+2] = 0; // set blue
      board__data[i+3] = 0; // set alpha
    }

    // set initial position of knight
    // board__data[(aKnightRow * board__width) + (aKnightCol * 4) + 0] = 1;
    board__data[0] = 1;

  // Generate, set up, and bind the texture
    texture_new(&board__texture, board__width, board__width, GL_TEXTURE_RECTANGLE_ARB, GL_RGBA32F_ARB, NULL);

    // initialize the FBO
    fbo_init(board__width, board__width);

    // Attach the texture to the framebuffer object
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, board__texture, 0);

    // Check the FBO for completeness
    fbo_check();

  // generate texture for board display
    texture_new(&board__display, board__width, board__width, GL_TEXTURE_2D, GL_RGBA, NULL);
}

void board_fini() {
  fbo_fini();
  glDeleteTextures(1, &board__display);
  glDeleteTextures(1, &board__texture);
  free(board__data);
}

void board_draw()
{
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1, -1);
    glTexCoord2f(1, 0); glVertex2f( 1, -1);
    glTexCoord2f(1, 1); glVertex2f( 1, 1);
    glTexCoord2f(0, 1); glVertex2f(-1, 1);
  glEnd();
}

void board_update()
{
  cgGLEnableProfile(cg__fragmentProfile);
  cgGLBindProgram(cg__fragmentProgram);

  // transfer data to texture
    texture_load_array(board__texture, GL_TEXTURE_RECTANGLE_ARB, board__width, board__width, board__data);

  // perform computation
    CGparameter textureCg = cgGetNamedParameter(cg__fragmentProgram, "aBoard");
    cgGLSetTextureParameter(textureCg, board__texture);
    cgGLEnableTextureParameter(textureCg);

    board_draw(); // GPGPU CONCEPT 4: Viewport-Sized Quad = Data Stream Generator.

    cgGLDisableTextureParameter(textureCg);
    cgGLDisableProfile(cg__fragmentProfile);

  // Read back the results
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    // The glReadBuffer function selects a color buffer source for pixels.
    // void glReadBuffer(GLenum mode); mode is a color buffer

    // The glReadPixels function reads a block of pixels from the framebuffer.
    glReadPixels(
      0, 0,           // GLint x, y The window coordinates of the first pixel that is read from the framebuffer. This location is the lower-left corner of a rectangular block of pixels
      board__width,   // GLsizei width
      board__width,   // GLsizei height
      GL_RGBA,        // GLenum format
      GL_FLOAT,       // GLenum type
      board__data);   // GLvoid *pixels
}

void board_display() {
  board_update();

  /*
  for(unsigned i = 0; i < board__width*board__width*4; ++i)
    printf("%6d: %30.15f\n", i % 4 ? i : 0, board__data[i]);

  printf("------");
  exit(0);
  */

  fbo_disable(); // render to screen, not the FBO
  texture_load_array(board__display, GL_TEXTURE_2D, board__width, board__width, board__data);
  glEnable(GL_TEXTURE_2D);
    board_draw();
  glDisable(GL_TEXTURE_2D);
  fbo_enable();
}
