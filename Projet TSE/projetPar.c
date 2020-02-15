#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
int N;

void *MARIT(void* arg) //Fonction qui calcul la moyenne arithmetique
{
	int *tab = (int *) arg;
	double somme = 0;
	for(int i=0;i<N;i++)
	{
		somme += *tab;
		tab++; //parcour le tableau
	}
	printf("\n\nLa moyenne arithmetique est %f \n",somme/N);
	pthread_exit(NULL); //ferme le thread
}

void *MABSO(void* arg) //Fonction qui calcul la moyenne absolue
{
	int *tab = (int *) arg;
	double somme=0;
	for(int i=0;i<N;i++)
	{
		somme+=abs(*tab);
		tab++; //parcour le tableau
	}
	printf("La moyenne absolue est %f \n", somme/N);
	pthread_exit(NULL); //ferme le thread
}

void *CARR(void* arg) //Fonction qui calcul la somme des carrés
{
	int *tab = (int *) arg;
	int somme=0;
	for(int i=0;i<N;i++)
	{
		somme+=(*tab)*(*tab);
		tab++; //parcour le tableau
	}
	printf("La somme des carrés est %d \n", somme);
	pthread_exit(NULL); //ferme le thread
}

void *SCUB(void* arg) //Foncrion la somme des carrés
{
	int *tab = (int *) arg;
	int somme=0;
	for(int i=0;i<N;i++)
	{
		somme+=(*tab)*(*tab)*(*tab);
		tab++; //parcour le tableau
	}
	printf("La somme des cubes est %d \n", somme);
	pthread_exit(NULL); //ferme le thread
}

int main(int argc,char* argv[])
{
	pthread_t tMARIT, tMABSO, tCARR, tSCUB; //Initialisation des threads

    struct timeval tvBegin,tvEnd;       //Structure qui recupére le temps en sec, ms
	struct timezone tzBegin,tzEnd;      // Structure qui recupére le type de changement horaire
	srand(time(NULL));                  // Initialisation du random

	while((N<=0)||(N>200))
	{
		printf("Donnez une valeur comprise entre 0 et 200 \n"); 
		scanf("%d",&N);
	}

	//on initialise le timer
	gettimeofday(&tvBegin,&tzBegin); //recupère le temps au début du programme
	printf("temps du debut  %d \n\n",tvBegin.tv_usec);

	int T[N];
	for(int i=0;i<N;i++) //remplissage du tableau en aleatoire
	{
		T[i]= rand() % (200+100)-100; 
		printf("[ %d ] ",T[i]);
	}


	if (pthread_create(&tMARIT, NULL, MARIT, &T)) //création du thread
	{
		perror("pthread_create");
		exit(1); //quitte le programme si la création du thread échoue
	}

	if (pthread_create(&tMABSO, NULL, MABSO, &T))
	{
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tCARR, NULL, CARR, &T))
	{
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tSCUB, NULL, SCUB, &T))
	{
		perror("pthread_create");
		exit(1);
	}

	if(pthread_join(tMARIT, NULL)) //rejoint le thread
	{
		perror("pthread_join");
		exit(1);
	}

	if(pthread_join(tMABSO, NULL))
	{
		perror("pthread_join");
		exit(1);
	}

	if(pthread_join(tCARR, NULL))
	{
		perror("pthread_join");
		exit(1);
	}

	if(pthread_join(tSCUB, NULL))
	{
		perror("pthread_join");
		exit(1);
	}

	gettimeofday(&tvEnd,&tzEnd); //recupère le temps à la fin du programme
	printf("temps final %d \n",tvEnd.tv_usec);

	//temps d'execution du programme
	printf("Le programme a mis %d microsecondes \n",tvEnd.tv_usec-tvBegin.tv_usec); //calcul du temps d'execution du programme

	return 0;
}

