#include "Board.h"

/******************************************************************************/
/* La estructura de un juego es simplemente un tablero y la cantidad de veces
que se va a iterar */
typedef struct _game {
  board_t *board;       // Tablero principal
  board_t *board_aux;   // Tablero auxiliar para siguiente generacion
  unsigned int cycles;  // numero de generaciones
}game_t;

/******************************************************************************/

/* Cargamos el juego desde un archivo */
game_t *loadGame(const char *filename);

/* Guardamos el tablero 'board' en el archivo 'filename' */
void writeBoard(board_t *board, const char *filename);
