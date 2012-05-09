CC = g++
CLANG = clang++

CFLAGS = -Wall -g
RELEASEFLAG = -Wall -O3 -Os -s

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lGLEW -lil

TARGET = PeGL
TARGETR = TeenSy

SRC = PeGL.cxx \
		shaderloader.cxx \
		imageloader.cxx \
		meshloader.cxx \
		objectmanager.cxx \
		
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
