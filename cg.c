#include <Cg/cg.h>
#include <Cg/cgGL.h>

CGcontext cg__context;
CGprofile cg__fragmentProfile;
CGprogram cg__fragmentProgram;

// just registers the program, does not activate it
void cg_register_fragment_program(const char* const aFileName, const char* const aFuncName)
{
  // create the fragment program
  cg__fragmentProgram = cgCreateProgramFromFile(
    cg__context,
    CG_SOURCE,
    aFileName,
    cg__fragmentProfile,
    aFuncName,
    NULL
  );

  if(cg__fragmentProgram != NULL)
    cgGLLoadProgram(cg__fragmentProgram);
}

void cg_catch_error(void)
{
  CGerror lastError = cgGetError();

  if(lastError)
  {
    printf("%s\n\n", cgGetErrorString(lastError));
    printf("%s\n", cgGetLastListing(cg__context));
  }
}

void cg_init() {
  cgSetErrorCallback(cg_catch_error);

  cg__context = cgCreateContext();

  cg__fragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  assert(cg__fragmentProfile != CG_PROFILE_UNKNOWN);

  // set optimal implicit compiler options
  cgGLSetOptimalOptions(cg__fragmentProfile);
}

void cg_fini() {
  cgDestroyContext(cg__context);
}
