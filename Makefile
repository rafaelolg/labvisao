# Criado por Rafael Lopes em 17 de agosto de 2010
SHELL = /bin/sh
CC=g++

#OPENGL
#LDFLAGS += -lm -lglut -lGL -lGLU 


#OPENCV
CFLAGS  += `pkg-config opencv --cflags`
LDFLAGS += `pkg-config opencv --libs`



CFLAGS += -g  -Wall
CFLAGS += -DNDEBUG

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)


PROGRAM:similar

all: $(PROGRAM)

similar.o: similar.cpp similar.h
histogram_similarity.o: histogram_similarity.cpp histogram_similarity.h

similar: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o similar

.cpp.o:
	$(CXX) $(CFLAGS)  -c $< -o $@


package:cleanall
	tar --exclude='*.svn*' -czvf pacote.tar.gz .


clean:
	rm -f $(OBJS) 

cleanall: clean
	rm -f  $(PROGRAM)


.PHONY:clean cleanall package
