#include "Board.h"

board_t *board_init(size_t col, size_t row){
  board_t *board = malloc(sizeof(struct _board));
  board->rows = row;
  board->columns = col;
  board->cell = malloc(row * sizeof(int *));
  for (int i = 0; i < row; i++)
    board->cell[i] = malloc(col * sizeof(int));

  return board;
}

int board_load(board_t *board, FILE *file){
  char temp[board->columns+1];

  for(int i = 0; i < board->rows; i++){
    // Leemos el archivo linea por linea,
    // almacenandola en el buffer temp
    fscanf(file, "%s", temp);
    // Traduce los O como 1 y las X como 0
    // para facilitar el calculo de vecinos vivos
    for (int j = 0; j < board->columns; j++)
      if (temp[j] == 'O')
        board->cell[i][j] = 1;
      else
        board->cell[i][j] = 0;
  }
}

void board_show(board_t *board){
  system("clear");
  for (int i = 0; i < board->rows; i++) {
    for (int j = 0; j < board->columns; j++)
      if (board->cell[i][j] == 1) {
        printf("\033[1;34m");
        //cambia el color de la impresion a azul
        putchar(ALIVE);
      } else {
        printf("\033[0m");
        //cambia el color de la impresion a blanco
        putchar(DEAD);
      }
    putchar('\n');
  }
  printf("\033[0m");
}

/* Destroy board */
void board_destroy(board_t *board){
  int rows = board->rows;
  for(int i = 0; i < rows; i++){
    free(board->cell[i]);
  }
  free(board->cell);
  free(board);
}
