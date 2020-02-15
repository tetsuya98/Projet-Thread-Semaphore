#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t s_lecture, s_fichier, s_ecriture;
const int N=100; //constante de taille des tableaux
int compteur=0; //file d'attente des lecteurs
int T[100], Tfinal[100], Tprime[100];

void *Lecteur(void* arg) //processus lecteur
{
	int *index = (int *) arg;
	printf("entr%ce : lecteur(%d)\n",130, *index);
	int j = rand() % (N-1) + 1; //entier aleatoire dans l'interval [1,N]
	sem_wait(&s_lecture); //P(s_lecture) il attend de pouvoir rejoindre la file d'attente
		compteur++; //il rejoint la file d'attente
		if(compteur == 1) { //si c'est le premier lecteur
			sem_wait(&s_fichier); //P(s_fichier) il attend de pouvoir accéder au tampon de T
		}
	sem_post(&s_lecture); //il a rejoint la file d'attente

	int lect = Tfinal[j]; //lecture dans T

	sem_wait(&s_lecture); //il attend de pouvoir revenir à la file d'attente
		compteur--; //il quitte la file d'attente
		if(compteur == 0) { //si il est le dernier de la file d'attente
			sem_post(&s_fichier); //il laisse l'accés au tampon de T aux écrivains
		}
	sem_post(&s_lecture); //il a quitté la file d'attente

	sem_wait(&s_ecriture); //il attend de pouvoir accéder au tampon de T'
		Tprime[j] = lect; //écriture dans T'
	sem_post(&s_ecriture); //il laisse l'accès au tampon de T' aux autres

	printf("sortie : lecteur(%d)\n", *index);
	pthread_exit(NULL); //on quitte le thread
}

void *Ecrivain(void* arg) //processus ecrivain
{
	int K = rand() % (N-1) + 1; //entier aleatoire dans l'interval [1,N]
	int *index = (int *) arg;
	printf("entr%ce : ecrivain(%d)\n", 130,*index);
	int alpha = rand() % ((3*N)-(2*N)) + (2*N); //entier aleatoire dans l'interval [2N,3N]
	sem_wait(&s_fichier); //il attend de pouvoir accéder au tampon de T
		Tfinal[K] = alpha; //écriture dans T
		int lect = Tfinal[(K+1)%N]; //lecture dans T
	sem_post(&s_fichier); //il libère l'accès au tampon T

	sem_wait(&s_ecriture); //il attend de pouvoir accéder au tampon de T'
		Tprime[(K+1)%N] = lect; //écriture dans T'
	sem_post(&s_ecriture); //il laisse l'accès au tampon de T' aux autres
	printf("sortie : ecrivain(%d)\n", *index);
	pthread_exit(NULL);
}


int main(int argc,char* argv[])
{
	pthread_t tLecteur, tEcrivain; //initialisation des threads
	//initialisation des semaphores
	sem_init(&s_lecture, 0, 1);
	sem_init(&s_fichier, 0, 1);
	sem_init(&s_ecriture, 0, 1);
	srand(time(NULL));
	int nb_lect = -1, nb_ecr = -1,test=0;//initialisation des variables

	for (int i = 0; i < N; i++) //remplissage des tableau
	{
		T[i]= rand() % 50; //remplissage aleatoire
		Tfinal[i] = T[i];
		Tprime[i] = 0;
	}

	printf("Combien voulez de : ");

	while(nb_lect < 0 || nb_lect >= 100) //choix du nombre de lecteurs
	{
		printf("\nLecteur ?");
		scanf("%d", &nb_lect);
	}

	while(nb_ecr < 0 || nb_ecr >= 100) //choix du nombre d'écrivains
	{
		printf("\nEcrivan ?");
		scanf("%d", &nb_ecr);
	}

	pthread_t tabEcrivain[nb_ecr];
	pthread_t tabLecteur[nb_lect];
	int indiceEcrivain[nb_ecr],indiceLecteur[nb_lect];

	printf("\n");

	for (int i = 0; i<nb_lect; i++) { //créé et rejoint les threads lecteurs
		indiceLecteur[i]=i;
		if (pthread_create(&tabLecteur[i], NULL, Lecteur,&indiceLecteur[i]))
		{
			perror("pthread_create");
			exit(1);
		}
	}

	for (int i = 0; i<nb_ecr; i++) {
		indiceEcrivain[i]=i;
		if (pthread_create(&tabEcrivain[i], NULL, Ecrivain, &indiceEcrivain[i])) //créé et rejoint les threads écrivain
		{
			perror("pthread_create");
			exit(1);
		}
	}

	for (int i = 0; i<nb_ecr; i++) {
		if(pthread_join(tabEcrivain[i],NULL))
		{
			perror("pthread_join");
			exit(1);
		}
	}

	for (int i = 0; i<nb_lect; i++) { //créé et rejoint les threads lecteurs
		if(pthread_join(tabLecteur[i], NULL))
		{
			perror("pthread_join");
			exit(1);
		}
	}

	printf("\n");

	printf("Tableau T initial : \n");
	for (int i = 0; i<N; i++) {
		printf("T[%d] = %d |", i, T[i]);
	}

	printf("\n\nTableau T final : \n");
	for (int i = 0; i<N; i++) {
		printf("T[%d] = %d |", i, Tfinal[i]);
	}

	printf("\n\nTableau T' final : \n");
	for (int i = 0; i<N; i++) {
		printf("T[%d] = %d |", i, Tprime[i]);
	}
	printf("\n");
	exit(0);
}
