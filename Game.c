#include "Game.h"

#define GETCELL(x) x == 1 ? ALIVE : DEAD
#define WORDSIZE

game_t *loadGame(const char *filename){
  game_t *game = malloc(sizeof(struct _game));

  // Obetenemos del archivo: nro de generaciones, filas y columnas del tablero
  FILE *f = fopen(filename, "r");
  size_t cycles, r, c;
  fscanf(f, "%lu %lu %lu", &cycles, &r, &c);
  game->cycles = cycles;

  // Generamos tableros
  game->board = board_init(c, r);
  game->board_aux = board_init(c, r);

  // Cargamos tablero del archivo de entrada al tablero principal
  board_load(game->board, f);

  fclose(f);
  return game;
}

void writeBoard(board_t *board, const char *filename){
  // Longitud de la direccion de la entrada sin contar "game"
  int len = strlen(strchr(filename, '.'));
  int corte = strlen(filename) - len + 1;
  char* salida = malloc(corte + len + 1);

  //copio toda la direccion de la entrada menos "game"
  strncpy(salida, filename, corte);

  //agrego "final" a la direccion de salida
  strcpy(&salida[corte], "final");

  FILE* fp = fopen(salida, "w");
  int rows = board->rows;
  int columns = board->columns;
  for(int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++)
      fputc(GETCELL(board->cell[i][j]), fp);
    fputc('\n', fp);
  }
  
  printf("\nArchivo %s creado con exito\n\n",salida);
  
  fclose(fp);
  free(salida);
}
