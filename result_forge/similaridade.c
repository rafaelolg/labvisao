#include<stdio.h>
#include<stdlib.h>
#include <math.h>

int modulo(int a)
{
  return a>0 ? a : -a;
}

int main(int argc, char **argv)
{
  FILE *ground, *teste;
  int i, j, k, gt[10][10], test[10][10];
  double soma[10];
  char nome[50];
  double total;
  double var;

  if(argc < 3){
    printf("Modo de usar: ./similarida referencia.txt teste.txt\n");
    return 1;
  }
  ground = fopen(argv[1], "r");
  teste = fopen(argv[2], "r");


  for(i=0; i<10; i++){
    fscanf(ground, "%s", nome);
    fscanf(teste, "%s", nome);
    for(j=0; j<10; j++){
      fscanf(ground, "%d", &gt[i][j]);
      fscanf(teste, "%d", &test[i][j]);
    }
  }

  for(i=0; i<10; i++){
    soma[i] = 0.0;
    for(j=0; j<10; j++){
      for(k=0; k<10; k++)
	if(test[i][j] == gt[i][k])
	  break;
      soma[i] += (11-(k+1))*(10-modulo(k-j));
    }
    soma[i] = soma[i] / 550.0;
  }

  printf("%s & %f", argv[2], soma[0]);
  total = soma[0];
  for(i=1; i<10; i++) {
    printf(" & %f", soma[i]);
    total+=soma[i];
  }
  var = 0;
  for (i = 0; i < 10;i++) {
      var += (total/10.0 - soma[i]) * (total/10.0 - soma[i]); 
  }
  var =sqrt(var/10.0);
  printf(" & %f & %f\n", total/10.0, var);
  fclose(ground);
  fclose(teste);
  return 0;
}
