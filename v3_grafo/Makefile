CC = gcc -Wall -O3

objs = ./src/floodit.o ./src/doubleQueue.o ./src/graph.o ./src/map.o ./src/solver.o

all: floodit

floodit: $(objs)
		$(CC) $(objs) -o floodit 

doubleQueue.o: ./src/doubleQueue.c ./includes/doubleQueue.h
		$(CC) ./src/doubleQueue.c

graph.o: ./src/graph.c ./includes/doubleQueue.h ./includes/graph.h ./includes/map.h ./src/graph.h
		$(CC) ./src/graph.c

map.o: ./src/map.c ./src/map.h
		$(CC) ./src/map.c

solver.o: ./src/solver.c ./includes/doubleQueue.h ./includes/graph.h ./src/solver.h
		$(CC) ./src/solver.c

clean:
	-rm -f ./src/*.o

purge: clean
	rm -f floodit
