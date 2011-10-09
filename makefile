CC = g++

CFLAGS = -Wall -O2
DEBUGFLAG = -g -Wall

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lGLEW

TARGET = OpenGL3
TARGETDBG = dbgOpenGL3

SRC = OpenGL3.cxx \
		Shader.cxx \
		Matrix.cxx \
		VBHandler.cxx \
		OBJLoader.cxx

OBJ = $(SRC:.cxx=.o)

all:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

compile:
	$(CC) -c $(CFLAGS) $(LDFLAGS) $(SRC)

build:
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJ)

debug:
	$(CC) $(DEBUGFLAG) $(SRC) $(LDFLAGS) -o $(TARGETDBG)

clean:
	rm *.o
