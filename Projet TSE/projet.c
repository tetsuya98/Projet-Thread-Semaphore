#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
int N;

void MARIT(int* pT)
{
	double somme=0;
	for(int i=0;i<N;i++)
	{
		somme+=pT[i];
	}
	printf("\n\nLa moyenne arithmetique est %f \n",somme/N);
}

void MABSO(int* pT)
{
	double somme=0;
	for(int i=0;i<N;i++)
	{
		somme+=abs(pT[i]);
	}
	printf("La moyenne absolue est %f \n",somme/N);
}

void CARRE(int* pT)
{
	int somme=0;
	for(int i=0;i<N;i++)
	{
		somme+=pT[i]*pT[i];
	}
	printf("La somme des carre est %d \n",somme);
}

void SCUB(int* pT)
{
	int somme=0;
	for(int i=0;i<N;i++)
	{
		somme+=pT[i]*pT[i]*pT[i];
	}
	printf("La somme des cube est %d \n",somme);
}

int main(int argc,char* argv[])
{
    struct timeval tvBegin,tvEnd;
	struct timezone tzBegin,tzEnd;
	
	srand(time(NULL));
	while((N<=0)||(N>200))
	{
		printf("Donnez une valeur comprise entre 0 et 200 \n");
		scanf("%d",&N);
	}

	//on initialise le timer
	gettimeofday(&tvBegin,&tzBegin);
	printf("temps du debut  %d \n\n",tvBegin.tv_usec);

	int T[N];
	for(int i=0;i<N;i++)
	{
		T[i]= rand() % (200+100)-100;
		printf("[ %d ] ",T[i]);
	}

	//Calcul de la moyenne arithmetique
	MARIT(T);

	//Calcul de la moyenne absolue
	MABSO(T);

	//Calcul de la somme des carre
	CARRE(T);

	//Calcul de la somme des cube
	SCUB(T);

	//timer final
	gettimeofday(&tvEnd,&tzEnd);
	printf("temps final %d \n",tvEnd.tv_usec);

	//temps d'execution du programme
	printf("Le programme a mis %d microsecondes \n",tvEnd.tv_usec-tvBegin.tv_usec);

	return 0;
}
