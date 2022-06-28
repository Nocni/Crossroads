#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include <semaphore.h>

#define clear() printf("\033[H\033[J")

sem_t semafor_a, semafor_b, semafor_c, semafor_d, raskrsnica_a, raskrsnica_b, raskrsnica_c, raskrsnica_d;

double ukupno_cekanje_a = 0;
double ukupno_cekanje_b = 0;
double ukupno_cekanje_c = 0;
double ukupno_cekanje_d = 0;
double ukupno_srednje_vreme = 0;
double optimalno_cekanje_a = 0;
double optimalno_cekanje_b = 0;
double optimalno_cekanje_c = 0;
double optimalno_cekanje_d = 0;
long int optimalno_svetlo_ac = 0;
long int optimalno_svetlo_bd = 0;
long int privremeno_svetlo_ac = 0;
long int privremeno_svetlo_bd = 0;

double prosecno_cekanje_a, prosecno_cekanje_b, prosecno_cekanje_c, prosecno_cekanje_d;

struct timespec spawn_a = {2, 727272720};
struct timespec spawn_b = {7, 500000000};
struct timespec spawn_c = {4, 285714280};
struct timespec spawn_d = {5, 000000000};
struct timespec cekanje_kola = {2, 000000000};
struct timespec trajanje_zelenog_ac = {30, 000000000};
struct timespec trajanje_zelenog_bd = {30, 000000000};

pthread_t kola_a, kola_b, kola_c, kola_d;

int suma_a = 0;
int suma_b = 0;
int suma_c = 0;
int suma_d = 0;

pthread_t smer_a, smer_b, smer_c, smer_d, kontrola_svetla;

void* vozi_a(void *_args)
{	
	sem_wait(&semafor_a);
	time_t vreme_kreiranja = time(NULL);
	sem_wait(&raskrsnica_a);

	nanosleep(&cekanje_kola, NULL);
	time_t vreme_prolaska = time(NULL);
	suma_a++;

	double razlika = vreme_prolaska - vreme_kreiranja;
	ukupno_cekanje_a += razlika;
	sem_post(&raskrsnica_a);

	pthread_exit( NULL );
}

void* vozi_b(void *_args)
{
	sem_wait(&semafor_b);
	time_t vreme_kreiranja = time(NULL);
	sem_wait(&raskrsnica_b);

	nanosleep(&cekanje_kola, NULL);
	time_t vreme_prolaska = time(NULL);
	suma_b++;

	double razlika = vreme_prolaska - vreme_kreiranja;
	ukupno_cekanje_b += razlika;
	sem_post(&raskrsnica_b);

	pthread_exit( NULL );
}

void* vozi_c(void *_args)
{	
	sem_wait(&semafor_c);
	time_t vreme_kreiranja = time(NULL);
	sem_wait(&raskrsnica_c);

	nanosleep(&cekanje_kola, NULL);
	time_t vreme_prolaska = time(NULL);
	suma_c++;

	double razlika = vreme_prolaska - vreme_kreiranja;
	ukupno_cekanje_c += razlika;
	sem_post(&raskrsnica_c);

	pthread_exit( NULL );
}

void* vozi_d(void *_args)
{
	sem_wait(&semafor_d);
	time_t vreme_kreiranja = time(NULL);
	sem_wait(&raskrsnica_d);

	nanosleep(&cekanje_kola, NULL);
	time_t vreme_prolaska = time(NULL);
	suma_d++;
	double razlika = vreme_prolaska - vreme_kreiranja;
	ukupno_cekanje_d += razlika;
	sem_post(&raskrsnica_d);

	pthread_exit( NULL );
}

double izracunaj(int k)
{
	// Optimalno resenje
	optimalno_cekanje_a = prosecno_cekanje_a;
	optimalno_cekanje_b = prosecno_cekanje_b;
	optimalno_cekanje_c = prosecno_cekanje_c;
	optimalno_cekanje_d = prosecno_cekanje_d;

	// Prosecno cekanje za svaki smer
	prosecno_cekanje_a = ukupno_cekanje_a / suma_a;
	prosecno_cekanje_c = ukupno_cekanje_c / suma_b;
	prosecno_cekanje_b = ukupno_cekanje_b / suma_c;
	prosecno_cekanje_d = ukupno_cekanje_d / suma_d;

	// Prosecno ukupno cekanje
	ukupno_srednje_vreme = ukupno_cekanje_a / (suma_a + suma_b + suma_c + suma_d) + ukupno_cekanje_b / (suma_a + suma_b + suma_c + suma_d) + ukupno_cekanje_c / (suma_a + suma_b + suma_c + suma_d) + ukupno_cekanje_d / (suma_a + suma_b + suma_c + suma_d);

	if(k)
		printf("\nCekanje a: %f \nCekanje b: %f\nCekanje c: %f\nCekanje d: %f\nUkupno cekanje: %f\n", prosecno_cekanje_a, prosecno_cekanje_b, prosecno_cekanje_c, prosecno_cekanje_d, ukupno_srednje_vreme);
	printf("Trajanje zelenog svetla AC: %ld \nTrajanje zelenog svetla BD: %ld\n", trajanje_zelenog_ac.tv_sec, trajanje_zelenog_bd.tv_sec);

	ukupno_cekanje_a = 0;
	ukupno_cekanje_b = 0;
	ukupno_cekanje_c = 0;
	ukupno_cekanje_d = 0;
	suma_a = 0;
	suma_b = 0;
	suma_c = 0;
	suma_d = 0;
	return ukupno_srednje_vreme;
}

void promeni_svetla(int k, double trenutno_vreme, double proslo_vreme)
{
	if(k)
	{
		optimalno_svetlo_ac = privremeno_svetlo_ac;
		optimalno_svetlo_bd = privremeno_svetlo_bd;
		privremeno_svetlo_ac = trajanje_zelenog_ac.tv_sec;
		privremeno_svetlo_bd = trajanje_zelenog_bd.tv_sec;
		if((trenutno_vreme > proslo_vreme) && (k > 1))
		{
			printf("\nOptimalno resenje : \nCekanje a: %f \nCekanje b: %f\nCekanje c: %f\nCekanje d: %f\nUkupno cekanje: %f\nTrajanje zelenog svetla AC: %ld \nTrajanje zelenog svetla BD: %ld\n", optimalno_cekanje_a, optimalno_cekanje_b, optimalno_cekanje_c, optimalno_cekanje_d, proslo_vreme, optimalno_svetlo_ac, optimalno_svetlo_bd);	
			exit(-1);
		}	
		if(k % 2 == 1)
		{
			if(trenutno_vreme > proslo_vreme)
				trajanje_zelenog_ac.tv_sec = trajanje_zelenog_ac.tv_sec + 4;
			else
				trajanje_zelenog_ac.tv_sec = trajanje_zelenog_ac.tv_sec - 4;
		}
		else
		{
			if(trenutno_vreme > proslo_vreme)
				trajanje_zelenog_bd.tv_sec = trajanje_zelenog_bd.tv_sec + 4;
			else
				trajanje_zelenog_bd.tv_sec = trajanje_zelenog_bd.tv_sec - 4;
		}
	}
	else
	{
		trajanje_zelenog_ac.tv_sec = trajanje_zelenog_ac.tv_sec - 4;
		trajanje_zelenog_bd.tv_sec = trajanje_zelenog_bd.tv_sec - 4;
	}
}

void* zeleno(void *_args)
{
	int i, j;
	int k = 0;
	double trenutno_vreme, proslo_vreme;

	while(1)
	{
		nanosleep(&trajanje_zelenog_ac, NULL);
		while(!sem_trywait(&semafor_a));
		while(!sem_trywait(&semafor_c));

		for(i = 0; i < trajanje_zelenog_bd.tv_sec / 2; i++)
		{	
			sem_post(&semafor_b);
			sem_post(&semafor_d);
		}

		nanosleep(&trajanje_zelenog_bd, NULL);
		while(!sem_trywait(&semafor_b));
		while(!sem_trywait(&semafor_d));

		/*Kraj iteracije*/
		trenutno_vreme = izracunaj(k);
		promeni_svetla(k, trenutno_vreme, proslo_vreme);
		proslo_vreme = trenutno_vreme;
		
		for(j = 0; j < trajanje_zelenog_ac.tv_sec / 2; j++)
		{
			sem_post(&semafor_a);
			sem_post(&semafor_c);
		}
		k++;
	}
	pthread_exit( NULL );
}

void* start(void *_args)
{
	int br_kola= *((int *) _args);

	switch(br_kola)
	{
		case 22:
			while(1)
			{
				pthread_create(&kola_a, NULL, &vozi_a, NULL);
				nanosleep(&spawn_a, NULL);
			}
		case 8:
			while(1)
			{
				pthread_create(&kola_b, NULL, &vozi_b, NULL);
				nanosleep(&spawn_b, NULL);
			}
		case 14:
			while(1)
			{
				pthread_create(&kola_c, NULL, &vozi_c, NULL);
				nanosleep(&spawn_c, NULL);
			}
		case 12:
			while(1)
			{
				pthread_create(&kola_d, NULL, &vozi_d, NULL);
				nanosleep(&spawn_d, NULL);
			}
	}
	pthread_exit( NULL );
}

int main(int argc, char *argv[])
{
	sem_init(&semafor_a, 0, 15);
	sem_init(&semafor_b, 0, 0);
	sem_init(&semafor_c, 0, 15);
	sem_init(&semafor_d, 0, 0);
	sem_init(&raskrsnica_a, 0, 1);
	sem_init(&raskrsnica_b, 0, 1);
	sem_init(&raskrsnica_c, 0, 1);
	sem_init(&raskrsnica_d, 0, 1);

	int a = 22;
	int b = 8;
	int c = 14;
	int d = 12;

	pthread_create(&kontrola_svetla, NULL, zeleno, NULL);
	pthread_create(&smer_a, NULL, start, (void *) &a);
	pthread_create(&smer_b, NULL, start, (void *) &b);
	pthread_create(&smer_c, NULL, start, (void *) &c);
	pthread_create(&smer_d, NULL, start, (void *) &d);

	pthread_join(smer_a, NULL);
	pthread_join(smer_b, NULL);
	pthread_join(smer_c, NULL);
	pthread_join(smer_d, NULL);

	return 0;
}
