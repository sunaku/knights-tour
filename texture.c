void texture_new(GLuint* aTexId, const GLuint aTexWidth, const GLuint aTexHeight, const float* const aTexData) {
  // generate texture
  glGenTextures(1, aTexId);

  // bind the texture
  glBindTexture(GL_TEXTURE_2D, *aTexId);

  // turn off filtering and set wrap mode
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // allocate and initialize graphics memory
  glTexImage2D(GL_TEXTURE_2D,
    0,              // mipmap level
    GL_RGBA, // format
    aTexWidth,      // size
    aTexHeight,
    0,              // no border
    GL_RGBA,        // internal format
    GL_FLOAT,       // format of data coming from the CPU
    aTexData);      // allocate and initialize
}

void texture_load_framebuffer(const GLuint aTexId, const GLuint aTexWidth, const GLuint aTexHeight) {
  glBindTexture(GL_TEXTURE_2D, aTexId);
  glCopyTexSubImage2D(
    GL_TEXTURE_2D, // only choice
    0,             // mipmap level
    0, 0,          // texel offsets in x and y
    0, 0,          // pixel offset from ll corner
    aTexWidth, aTexHeight  // width and heigth of sub-image to copy
  );
}
