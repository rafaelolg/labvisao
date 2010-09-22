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



PROGRAM:similar calcula_similaridade_entre

all: $(PROGRAM)

histogram_similarity.o: histogram_similarity.cpp histogram_similarity.h
histogram_similarity.o: histogram_similarity.cpp histogram_similarity.h similar.h
localized_similarity.o: localized_similarity.cpp localized_similarity.h similar.h
moments_similarity.o: moments_similarity.cpp moments_similarity.h similar.h
similar.o: similar.cpp similar.h surf_similarity.h histogram_similarity.h localized_similarity.h moments_similarity.h
surf_similarity.o: surf_similarity.cpp surf_similarity.h similar.h



calcula_similaridade_entre: histogram_similarity.o localized_similarity.o moments_similarity.o calcula_similaridade_entre.o surf_similarity.o
	$(CXX) $(LDFLAGS) $^ -o $@


similar: histogram_similarity.o localized_similarity.o moments_similarity.o similar.o surf_similarity.o
	$(CXX) $(LDFLAGS) $^ -o $@

.cpp.o:
	$(CXX) $(CFLAGS)  -c $< -o $@


package:cleanall
	tar --exclude='*.svn*' -czvf pacote.tar.gz .


clean:
	rm -f *.o $(PROGRAM)



.PHONY:clean cleanall package
