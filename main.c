// CMPE-220, Winter 2007, final project
// Knight's Tour in Cg
// Suraj Kurapati

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "glew.c"
#include "glut.c"
#include "cg.c"
#include "texture.c"
#include "board.c"

void main_display(void)
{
  board_display();
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  // set up
    glut_init(argc, argv, "Knight's Tour in Cg", 512, 512);
    glew_init();

    cg_init();
    cg_register_fragment_program("naive.cg", "main");

  // ask for parameters
    printf(
      "\n// CMPE-220, Winter 2007, final project"
      "\n// Knight's Tour in Cg"
      "\n// Suraj Kurapati"
      "\n\n"
    );

    unsigned width, row, col;
    printf("chess board's width?      "); scanf("%u", &width);
    printf("knight's starting row?    "); scanf("%u", &row);
    printf("knight's starting column? "); scanf("%u", &col);

    if (row >= width || col >= width) {
      printf("Error: row and column must be in range [0, %u).", width);
      exit(EXIT_FAILURE);
    }

  // run algorithm
    board_init(width, row, col);
    glutDisplayFunc(main_display);
    glutMainLoop();

  // tear down
    board_fini();
    cg_fini();
    glew_fini();

  return 0;
}
