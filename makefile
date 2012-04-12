CC = g++
CLANG = clang++

CFLAGS = -Wall -g
RELEASEFLAG = -Wall -O3 -s
DEBUGFLAG = -g -Wall

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lGLEW

TARGET = OpenGL3
TARGETDBG = dbgOpenGL3
TARGETR = TeenSy
TARGETP = Particle

SRC = OpenGL3.cxx \
		Shader.cxx \
		Matrix.cxx \
		VBHandler.cxx \
		OBJLoader.cxx \
		Particle.cxx \
		meshmanager.cxx \
		texture.cxx
		
OBJ = $(SRC:.cxx=.o)

all:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

compile:
	$(CC) -c $(CFLAGS) $(LDFLAGS) $(SRC)

build:
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJ)

debug:
	$(CC) $(DEBUGFLAG) $(SRC) $(LDFLAGS) -o $(TARGETDBG)
	
release:
	$(CC) $(RELEASEFLAG) $(SRC) $(LDFLAGS) -o $(TARGETR)

clean:
	rm *.o
clang:
	$(CLANG) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)
