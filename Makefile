CC = g++
LIBS = -lGL -lGLU -lglut

default:
	$(CC) tetris.cpp -o tetris $(LIBS)

clean:
	rm tetris
