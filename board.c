#include "fbo.c"

#define board__area (board__width * board__width)
GLuint board__width;
GLuint board__computeTexture;
GLuint board__displayTexture;
GLuint board__gridDisplayList;
unsigned long board__numCycles;
float* board__data;

void board_init(const GLuint aWidth, const GLuint aKnightRow, const GLuint aKnightCol)
{
  board__width = aWidth;

  // number of cycles it takes to finish the computation:
  //
  //   (3 cycles per knight) * (number of knights) - 1
  //
  // the -1 is because the initial knight is not determined
  // at runtime, so we save one cycle of processing.
  board__numCycles = (3 * board__area) - 1;
  printf("\nThe tour should take %lu cycles to complete.\n", board__numCycles);

  // generate the board data
    if((board__data = (float*)malloc(4*board__area*sizeof(float))) == NULL)
    {
      printf("\n*** out of memory allocating board__data ***\n");
      exit(2);
    }

    unsigned addr = 0;
    for(unsigned row = 0; row < board__width; row++) {
      for (unsigned col = 0; col < board__width; col++) {
        // red channel
        board__data[addr++] = (
          // set initial position of knight
          row == aKnightRow &&
          col == aKnightCol
        ) ? 1 : 0;

        // green channel
        board__data[addr++] = 0;

        // blue channel
        board__data[addr++] = 0;

        // alpha channel
        board__data[addr++] = 8; // 8 is MOVE_NONE (see naive.cg.erb for details); all cells should have next move = NONE initially, because only the knight makes the "next move" desicison at *runtime*
      }
    }

  // Generate, set up, and bind the texture
    texture_new(&board__computeTexture, board__width, board__width, GL_TEXTURE_RECTANGLE_ARB, GL_RGBA32F_ARB, NULL);

    // initialize the FBO
    fbo_init(board__width, board__width);

    // Attach the texture to the framebuffer object
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, board__computeTexture, 0);

    // Check the FBO for completeness
    fbo_check();

  // generate texture for board display
    texture_new(&board__displayTexture, board__width, board__width, GL_TEXTURE_2D, GL_RGBA, NULL);

  // generate OpenGL display lists to speed up rendering
    board__gridDisplayList = glGenLists(1);

    glNewList(board__gridDisplayList, GL_COMPILE);
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
    glEndList();
}

void board_fini() {
  fbo_fini();
  glDeleteLists(board__gridDisplayList, 1);
  glDeleteTextures(1, &board__displayTexture);
  glDeleteTextures(1, &board__computeTexture);
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
    texture_load_array(board__displayTexture, GL_TEXTURE_2D, board__width, board__width, board__data);
    glEnable(GL_TEXTURE_2D);
      board_draw();
    glDisable(GL_TEXTURE_2D);

  // draw board outline (grid)
    glCallList(board__gridDisplayList);

  fbo_enable();
}

void board_update()
{
  cgGLEnableProfile(cg__fragmentProfile);
  cgGLBindProgram(cg__fragmentProgram);

  // transfer data to texture
    texture_load_array(board__computeTexture, GL_TEXTURE_RECTANGLE_ARB, board__width, board__width, board__data);

  // perform computation
    CGparameter textureCg = cgGetNamedParameter(cg__fragmentProgram, "aBoard");
    cgGLSetTextureParameter(textureCg, board__computeTexture);
    cgGLEnableTextureParameter(textureCg);

    CGparameter widthCg = cgGetNamedParameter(cg__fragmentProgram, "aWidth");
    cgSetParameter1f(widthCg, board__width);

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
  board_render();

  // handle algorithm completion
  static unsigned long cycle = 0;
  cycle++;

  if (cycle >= board__numCycles) {
    printf("\nThe tour should now be complete.");

    #ifdef WIN32
    printf("\n\nPress the ENTER key to continue.");
    fflush(stdin);
    int c = getchar();
    #endif

    printf("\nThe Knight toured in this order:\n");

    // dump out the board contents
      unsigned long numSkipped = 0;

      unsigned addr = 0;
      for(unsigned row = 0; row < board__width; row++) {
        printf("\n");

        for (unsigned col = 0; col < board__width; col++) {
          float step = board__data[addr];
          addr += 4;

          if (step == 0) {
            numSkipped++;
            printf(" %3c", '#');
          }
          else {
            printf(" %3.f", step);
          }
        }
      }
      printf("\n");

    if (numSkipped > 0) {
      printf("\nThe Knight failed to visit %lu cells.\n", numSkipped);
      exit(EXIT_FAILURE);
    }
    else {
      exit(EXIT_SUCCESS);
    }
  }
}
