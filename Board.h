#ifndef BOARD
#define BOARD

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Representamos las células vivas como 'O' y las muertas como 'X' */
#define ALIVE 'O'
#define DEAD 'X'

/******************************************************************************/
/* Definición de la estructura de datos del tablero */

typedef struct _board{
  int **cell;   // Matriz del tablero
  int rows;     // Filas del tablero
  int columns;  // Columnas del tablero
}board_t;

typedef struct _b_data{
  board_t *d_board;       // Tablero principal
  board_t *d_board_aux;   // Tablero auxiliar
  int start;              // index para el thread
  int div;                // divisor para cantidad de threads
  int cycle;              // cant generaciones
}b_data;
/******************************************************************************/
/******************************************************************************/

/* Funciones sobre el tablero */

/* Creación del tablero */
board_t *board_init(size_t col, size_t row);

/* Leer de una lista de caracteres e interpretarla como un tablero */
int board_load(board_t *board, FILE *file);

/* Función para mostrar el tablero */
/* La función 'board_show' asume que hay espacio suficiente en 'res' para alojar el tablero.*/
void board_show(board_t *board);

/* Destroy board */
void board_destroy(board_t *board);
#endif
