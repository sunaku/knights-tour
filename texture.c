void texture_new(GLuint* aTexId, const GLuint aTexWidth, const GLuint aTexHeight, const GLenum aTexType, const int aTexFormat, const float* const aTexData) {
  // generate texture
  glGenTextures(1, aTexId);

  // bind the texture
  glBindTexture(aTexType, *aTexId);

  // turn off filtering and set wrap mode
  glTexParameteri(aTexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(aTexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(aTexType, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(aTexType, GL_TEXTURE_WRAP_T, GL_CLAMP);

  // allocate and initialize graphics memory
  glTexImage2D(aTexType,
    0,              // mipmap level
    aTexFormat,     // format
    aTexWidth,      // size
    aTexHeight,
    0,              // no border
    GL_RGBA,        // internal format
    GL_FLOAT,       // format of data coming from the CPU
    aTexData);      // allocate and initialize
}

void texture_load_screen(const GLuint aTexId, const GLenum aTexType, const GLuint aTexWidth, const GLuint aTexHeight, const float* const aTexData) {
  glBindTexture(aTexType, aTexId);
  glCopyTexSubImage2D(
    aTexType, // only choice
    0,             // mipmap level
    0, 0,          // texel offsets in x and y
    0, 0,          // pixel offset from ll corner
    aTexWidth, aTexHeight  // width and heigth of sub-image to copy
  );
}

void texture_load_array(const GLuint aTexId, const GLenum aTexType, const GLuint aTexWidth, const GLuint aTexHeight, const float* const aTexData) {
  glBindTexture(aTexType, aTexId);
  glTexSubImage2D(
    aTexType, // GLenum target
    0, // GLint mipmap level
    0, // GLint xoffset in the x direction within the texture array
    0, // GLint yoffset in the y direction within the texture array
    aTexWidth, // GLsizei width of the texture sub-image
    aTexHeight, // GLsizei height of the texture sub-image
    GL_RGBA, // GLenum format of the pixel data
    GL_FLOAT, // GLenum type of the pixel data
    aTexData // const GLvoid *pixels - A pointer to the image data in mem
  );
}
