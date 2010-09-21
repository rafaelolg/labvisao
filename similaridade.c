#include<stdio.h>
#include<stdlib.h>

int modulo(int a)
{
  return a>0 ? a : -a;
}

int main(int argc, char **argv)
{
  FILE *ground, *teste;
  if(argc < 3){
    printf("Modo de usar: ./similarida referencia.txt teste.txt\n");
    return 1;
  }
  ground = fopen(argv[1], "r");
  teste = fopen(argv[2], "r");
  int i, j, k, gt[10][10], test[10][10], soma[10];
  char nome[50];


  for(i=0; i<10; i++){
    fscanf(ground, "%s", nome);
    fscanf(teste, "%s", nome);
    for(j=0; j<10; j++){
      fscanf(ground, "%d", &gt[i][j]);
      fscanf(teste, "%d", &test[i][j]);
    }
  }

  for(i=0; i<10; i++){
    soma[i] = 0;
    for(j=0; j<10; j++){
      for(k=0; k<10; k++)
	if(test[i][j] == gt[i][k])
	  break;
      soma[i] += (11-(k+1))*(10-modulo(k-j));
    }
  }

  printf("%s:\n%d", argv[2], soma[0]);
  for(i=1; i<10; i++)
    printf(" & %d", soma[i]);
  printf("\n");

  close(ground);
  close(teste);
  return 0;
}
