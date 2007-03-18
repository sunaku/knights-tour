#include "fbo.c"

#define board__area (board__width * board__width)
GLuint board__width;
GLuint board__texture;
GLuint board__display;
float* board__data;

void board_init(const GLuint aWidth, const GLuint aKnightRow, const GLuint aKnightCol)
{
  board__width = aWidth;

  // generate the board data
    if((board__data = (float*)malloc(4*board__area*sizeof(float))) == NULL)
    {
      printf("\n*** out of memory allocating board__data ***\n");
      exit(2);
    }

    unsigned addr = 0;
    for(unsigned row = 0; row < board__width; row++) {
      for (unsigned col = 0; col < board__width; col++) {
        for (unsigned chan = 0; chan < 4; chan++) {
          board__data[addr] = (
            // set initial position of knight
            row == aKnightRow &&
            col == aKnightCol &&
            chan == 0 // red channel
          ) ? board__area : 0;

          addr++;
        }
      }
    }

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

void board_render() {
  fbo_disable(); // render to screen, not the FBO

  // draw board contents
    texture_load_array(board__display, GL_TEXTURE_2D, board__width, board__width, board__data);
    glEnable(GL_TEXTURE_2D);
      board_draw();
    glDisable(GL_TEXTURE_2D);

  // draw board outline (grid)
    float step = 2.0 / board__width;
    float cell;

    glBegin(GL_LINES);
      // draw rows
      for (cell = -1 + step; cell < board__width; cell += step) {
        glVertex3f(-1, cell, -1);
        glVertex3f(1, cell, -1);
      }

      // draw columns
      for (cell = -1 + step; cell < board__width; cell += step) {
        glVertex3f(cell, -1, -1);
        glVertex3f(cell, 1, -1);
      }
    glEnd();

  fbo_enable();
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
  // dump out the board contents
    unsigned addr = 0;
    for(unsigned row = 0; row < board__width; row++) {
      for (unsigned col = 0; col < board__width; col++) {
        for (unsigned chan = 0; chan < 4; chan++) {
          printf("y %u, x %u, channel %d: %30.15f\n", row, col, chan, board__data[addr]);
          addr++;
        }
        printf("\n");
      }
    }
    printf("======\n");
  */

  board_render();
}
