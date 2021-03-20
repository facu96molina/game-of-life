Implementación del Juego de la Vida de Conway

Game.h
Para la implementación de la estructura game_t, añadimos un tablero auxiliar donde guardar los estados calculados en cada ciclo para la siguiente generación del tablero.

Game.c
En la ultima generación del juego, se muestra el estado final en pantalla y se lo guarda en un archivo con la función writeboard, que abre el archivo con el mismo nombre que la entrada a diferencia de la terminación .final en lugar de .game.

Board.h
Para implementar el tablero, board_t contiene cell (un arreglo de arreglos de enteros), rows (el numero de filas), y columns (el numero de columnas). El arreglo cell usa enteros (1/0) en lugar de caracteres (O/X) para facilitar la cuenta de vecinos vivos de una célula a la hora de actualizar el tablero.
Tambien se declaró una estructura b data con todos los datos que cada thread necesita, para poder pasarlos en pthread create en un solo argumento.

Board.c
En este archivo lo mas importante es la implementación de board show, que imprime las células vivas en color azul para diferenciarlas de las muertas y poder apreciar mejor la evolución.

barrera.h
Utilizamos una barrera de condición para lograr que los n hilos que estemos utilizando, sincronicen su trabajo en distintas partes de la función a ejecutar.

Esto se logra creando una variable de condición, la cual nos permite esperar a que terminen todos los hilos antes de seguir a la siguiente sección de la función.

pthread_cond_init inicializa la variable de condición, pthread cond wait suspende la ejecución del hilo temporalmente y desbloquea el mutex para evitar deadlocks, y pthread cond broadcast reinicia todos los threads que esperan una condición de variable.

main.c
Este archivo contiene las funciones principales. Se carga el juego y se le pide al usuario ingresar un entero divisor de la cantidad de filas, para generar ese numero de hilos que puedan trabajar sobre partes diferentes del tablero.

Para crear los hilos, se usa un arreglo de estructuras b data (ya que cada hilo debe trabajar sobre sectores de memoria diferentes), y la función thread function que realizará la mayor parte del proceso.

En cada ciclo, cada hilo calcula la evolución de su sección del tablero y luego espera que todos los hilos terminen para poder seguir a actualizar los valores. La espera se hace con una barrera (implementada en barrera.h) la cual utiliza variables mutex y cond. El uso de barreras evita deadlocks y evoluciones incorrectas.
