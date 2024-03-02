all: air

LIBS= -lglut -lGL -lGLU

OS := $(shell uname)
ifeq ($(OS),Darwin)
 LIBS= -framework Carbon -framework OpenGL -framework GLUT 
endif

air: air-resistance.o
	g++ air-resistance.o -o air $(LIBS)

air-resistance.o: air-resistance.cc
	g++ -c air-resistance.cc -Wno-deprecated -DGL_SILENCE_DEPRECATION

clean:
	rm *.o
