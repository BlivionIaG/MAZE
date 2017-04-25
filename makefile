SRC_DIR = src/

CFLAGS = -g -Wall -Wextra -std=c11 -fdiagnostics-color=auto
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lGL -lGLU

MAZE_SOLV = maze_solver
MAZE_GEN = maze_generator

all : $(MAZE_SOLV).bin $(MAZE_GEN).bin
	rm -f *.o

$(MAZE_SOLV).bin : maze_solver.o lymbe.o ssdl.o
	gcc $(CFLAGS) maze_solver.o lymbe.o ssdl.o $(LDFLAGS) -o $(MAZE_SOLV).bin
$(MAZE_GEN).bin : maze_generator.o mazemaker.o
	gcc $(CFLAGS) maze_generator.o mazemaker.o -o $(MAZE_GEN).bin

maze_solver.o : $(SRC_DIR)maze_solver.c $(SRC_DIR)lymbe.h
	gcc $(CFLAGS) -c $(SRC_DIR)maze_solver.c
lymbe.o : $(SRC_DIR)lymbe.c $(SRC_DIR)lymbe.h
	gcc $(CFLAGS) -c $(SRC_DIR)lymbe.c
ssdl.o : $(SRC_DIR)ssdl.c $(SRC_DIR)ssdl.h
	gcc $(CFLAGS) -c $(SRC_DIR)ssdl.c

maze_generator.o : $(SRC_DIR)maze_generator.c $(SRC_DIR)mazemaker.h
	gcc $(CFLAGS) -c $(SRC_DIR)maze_generator.c
mazemaker.o : $(SRC_DIR)mazemaker.c $(SRC_DIR)mazemaker.h
	gcc $(CFLAGS) -c $(SRC_DIR)mazemaker.c

clean:
	rm -f *.o *.gch *.bin
