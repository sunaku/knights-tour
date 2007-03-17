// Suraj Kurapati
// CATS id: skurapat
//
// CMPE-220, Winter 2007, final project
// Knight's Tour in Cg

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "glew.c"
#include "glut.c"
#include "cg.c"
#include "fbo.c"
#include "texture.c"
#include "board.c"

#define SIZE 8

void main_display(void)
{
  board_display();
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  // set up
    glut_init(argc, argv, "Knight's Tour in Cg", SIZE, SIZE);

    glew_init();
    fbo_init(SIZE, SIZE);

    cg_init();
    cg_register_fragment_program("naive.cg", "main");

  // run algorithm
    board_init(SIZE, 0, 0);
    glutDisplayFunc(main_display);
    glutMainLoop();

  // tear down
    board_fini();
    cg_fini();
    fbo_fini();
    glew_fini();

  return 0;
}
