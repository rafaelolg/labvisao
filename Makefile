# Criado por Rafael Lopes em 17 de agosto de 2010
SHELL = /bin/sh
CC=g++

#OPENGL
#LDFLAGS += -lm -lglut -lGL -lGLU 


#OPENCV
CFLAGS  += `pkg-config opencv --cflags`
LDFLAGS += `pkg-config opencv --libs`



CFLAGS += -g  -Wall -Wpointer-arith
CFLAGS += -Wmissing-declarations
CFLAGS += -fno-strict-aliasing 
CFLAGS += -DNDEBUG

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)

#INCLUIR gcc -MM *.cpp

PROGRAM:similar

all: $(PROGRAM)

similar:similar.o
	$(CC) $(LDFLAGS) $< -o $@


.cpp.o:
	$(CC) $(CFLAGS)  -c $< -o $@



package:cleanall
	tar --exclude='*.svn*' -czvf pacote.tar.gz .


clean:
	rm -f $(OBJS) 

cleanall: clean
	rm -f  $(PROGRAM)


.PHONY:clean cleanall package
