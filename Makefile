DIRJUEGO = Ejemplo.game3
PROG = simulador

all: $(PROG) $(DIRJUEGO)
	./$(PROG) $(DIRJUEGO)
	rm $(PROG)

$(PROG): main.c Board.c Board.h Game.c Game.h barrera.h
	gcc main.c Board.c Game.c -o $(PROG) -lpthread
