GLuint board__width;
GLuint board__texture;

void board_init(const GLuint aWidth, const GLuint aKnightRow, const GLuint aKnightCol)
{
  unsigned i;
  float* data;

  board__width = aWidth;

  // generate the board data
    if((data = (float*)malloc(4*board__width*board__width*sizeof(float))) == NULL)
    {
      printf("\n*** out of memory allocating data ***\n");
      exit(2);
    }

    for (i = 0; i < board__width*board__width*4; i += 4)
    {
      data[i+0] = 0; //(rand() >= (RAND_MAX/4) ? 1.0 : 0.0); // set red
      data[i+1] = 0; // set green
      data[i+2] = 0; // set blue
      data[i+3] = 0; // set alpha
    }

  // set initial position of knight
    // data[(board__width * aKnightRow) + (4 * aKnightCol) + 1] = 1;
    data[1] = 1;

  texture_new(&board__texture, board__width, board__width, data);

  free(data);
}

void board_fini() {
  glDeleteTextures(1, &board__texture);
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
  CGparameter textureCg;
  textureCg = cgGetNamedParameter(cg__fragmentProgram, "aBoard");

  cgGLEnableProfile(cg__fragmentProfile);
  cgGLBindProgram(cg__fragmentProgram);

  // give input to simulation
    cgGLSetTextureParameter(textureCg, board__texture);
    cgGLEnableTextureParameter(textureCg);

  // GPGPU CONCEPT 4: Viewport-Sized Quad = Data Stream Generator.
  // perform computation
    board_draw();

  cgGLDisableTextureParameter(textureCg);
  cgGLDisableProfile(cg__fragmentProfile);

  // copy results of simulation from back buffer to texture
    glBindTexture(GL_TEXTURE_2D, board__texture);
    glCopyTexSubImage2D(
      GL_TEXTURE_2D, // only choice
      0,             // mipmap level
      0, 0,          // texel offsets in x and y
      0, 0,          // pixel offset from ll corner
      board__width, board__width  // width and heigth of sub-image to copy
    );
}

void board_display() {
  board_update();

  glBindTexture(GL_TEXTURE_2D, board__texture);
  glEnable(GL_TEXTURE_2D);
    board_draw();
  glDisable(GL_TEXTURE_2D);
}
