#include <unistd.h>
#include <time.h>
#include "Game.h"
#include "Board.h"
#include "barrera.h"

#define ESPERA 4000

barrier_t barr;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

//guarda los datos necesarios para cada hilo en un arreglo de estructuras b_data
b_data **loadArgs(b_data **data, game_t *game, int div){
  for(int i = 0; i < div; i++){
    data[i] = malloc(sizeof(b_data));
    data[i]->d_board = game->board;
    data[i]->d_board_aux = game->board_aux;
    data[i]->start = i;
    data[i]->div = div;
    data[i]->cycle = game->cycles;
  }

  return data;
}

//calcula el indice siguiente a n en base al total
int sig(int n, int total) {
  return (n + 1) % total;
}

//calcula el indice anterior a n en base al total
int ant(int n, int total) {
  if (n == 0)
    return total-1;
  return n-1;
}

//Calcula la cantidad de vecinos vivos de la celula x,y
int contar_vecinos(board_t *board, int x, int y) {
  int n = 0, i, j;
  int cont1 = 0, cont2;
  //contadores para la cantidad de filas/columnas que se iteran

  for (i = ant(x, board->rows); cont1 < 3; i = sig(i, board->rows), cont1++) {
    cont2 = 0;
    for (j = ant(y, board->columns); cont2 < 3; j = sig(j, board->columns), cont2++) {
      if (i == x && j == y)
        continue;
      else
        n += board->cell[i][j];
        //se suma el valor de la celda al total
        //1 si esta viva y 0 si esta muerta
    }
  }

  return n;
}

//calcula la evolucion de bias filas del tablero desde la fila start
//y guarda los valores en las mismas filas del tablero auxiliar
void *cambiar_valor(board_t *board, board_t *board_aux, int bias, int start) {
  int vecinos;

  for (int i = start; i < start + bias; i++)
    for (int j = 0; j < board->columns; j++){
      vecinos = contar_vecinos(board, i, j);
      //cuenta los vecinos vivos
      if (board->cell[i][j] == 1 && (vecinos < 2 || vecinos > 3))
        board_aux->cell[i][j] = 0;
        //si esta viva y la cantidad de vecinos vivos es
        //diferente de 2 o 3, muere
      else if (board->cell[i][j] == 0 && vecinos == 3)
        board_aux->cell[i][j] = 1;
        //si esta muerta y tiene 3 vecinos vivos exactamente
        //revive
    }
}

//actualiza bias filas del tablero desde la fila start
//con los valores guardados en el tablero auxiliar
void *board_update(board_t *b, board_t *b_aux, int bias, int start) {
  for (int i = start; i < start + bias; i++)
    for (int j = 0; j < b->columns; j++)
      b->cell[i][j] = b_aux->cell[i][j];
}

//funcion que se pasa a los hilos en pthread_create
//que recibe un puntero a b_data en forma de puntero a void,
//calcula las filas del tablero sobre las que va a trabajar el hilo
//e itera sobre los ciclos para actualizar esas filas
void *thread_function(void *args){
  b_data *data = (b_data *) args;

  int bias = data->d_board->rows / data->div;
  int start = bias * data->start;

  int cycles = data->cycle;
  for (int i = 0; i < cycles; i++){
    //calcula la evolucion de su seccion del tablero
    cambiar_valor(data->d_board, data->d_board_aux, bias, start);
    //espera a los otros hilos
    barrier_wait(&barr);

    //actualiza la seccion
    board_update(data->d_board, data->d_board_aux, bias, start);
    //espera a los otros hilos para iterar de nuevo
    barrier_wait(&barr);
    pthread_mutex_lock(&mut);
    usleep(ESPERA);
    board_show(data->d_board);
    pthread_mutex_unlock(&mut);
  }
}


int main(int argc, char const *argv[]){
  if (argc != 2){
    // La direccion del archivo de entradas se debe pasar como argumento
    printf("Error, uso: %s <Archivo de Entrada>\n", argv[0]);
    exit(1);
  }

  // Carga el juego desde el archivo
  game_t *game = loadGame(argv[1]);


  board_update(game->board_aux, game->board, game->board->rows, 0);
  int rows = game->board->rows, n_threads;

  do{
    printf("\nIngrese la cant de threads a usar, donde la cant divida a %d: ", rows);
    scanf("%d", &n_threads);
  }while (game->board->rows % n_threads != 0);
  //pide una cantidad que divida a las filas
  //para separarlas entre los hilos

  pthread_t threads[n_threads];
  barrier_init(&barr, n_threads);

  // Se crea un arreglo de b_data para pasar los argumentos
  // a cada hilo en espacios separados de memoria
  b_data **data = malloc(n_threads * sizeof(b_data));
  data = loadArgs(data, game, n_threads);

  for (int i = 0; i < n_threads; i++)
    pthread_create(&threads[i], NULL, &thread_function, (void *)data[i]);

  for(int i = 0; i < n_threads; i++)
    pthread_join(threads[i], NULL);

  barrier_destroy(&barr);

  // La ultima generacion se guarda en un archivo
  writeBoard(game->board, argv[1]);

  // Liberamos memoria usada
  board_destroy(game->board);
  board_destroy(game->board_aux);

  free(game);
  for(int i = 0; i < n_threads; i++)
    free(data[i]);
  free(data);

  return 0;
}
