#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "graphic.h"
#include <sys/sysinfo.h>

#define N 200
//#define core (int)system("grep 'model name' /proc/cpuinfo | wc -l")
#define core get_nprocs()

//LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/graphic/usr/lib
//export LD_LIBRARY_PATH

// optimisation: nombre de threads = nombre de coeurs
// -> grep 'model name' /proc/cpuinfo | wc -l

/* structure de point */
typedef struct
{
	int x,y,color;
} Point;

/* structure parametre_thread */
typedef struct
{
	pthread_mutex_t *mutex;
	Point *point;
} Parametre_thread;

/* code des threads */
void *code_thread (void *param)
{
	// récupération des paramètres
	Parametre_thread *p = (Parametre_thread *) param;
	// boucle infinie
	while(1)
	{
		
		//pour chaque point du tableau de paramètres, on traite le point
		for(int i = 0; i<N/core;i++)
		{
			pthread_mutex_lock(p->mutex);
			p->point[i].x+=g_random()%2?1:-1;
			p->point[i].y+=g_random()%2?1:-1;
			pthread_mutex_unlock(p->mutex);
		}

		g_msleep(20);

	}
}

/* main */
int main()
{
	// tableau de points
	Point points[N];
	// incrementeur
	int i;
	// mutex
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutex_attr;
	pthread_mutex_init(&mutex, &mutex_attr);

	Parametre_thread parametres[core];

	/* initialisation aléatoire des N points */
	for(i=0; i<N; ++i)
	{
		points[i].x=g_random()%(G_WIDTH-200)+100;
		points[i].y=g_random()%(G_HEIGHT-200)+100;
		points[i].color=g_random()%G_NB_COLORS;
	}


	for(i=0; i<core; ++i)
	{
		// on remplit le tableau de paramètres
		parametres[i].mutex = &mutex;
		parametres[i].point = &points[i*N/core];

		// on crée le thread
		pthread_t thread;
		if(pthread_create(&thread, NULL, code_thread, (void *) &parametres[i]) < 0)
		{
			fprintf (stderr, "Erreur création thread\n");
			exit (1);
		}

	}


	/* Affichage des points */
	g_init();
	while(1)
	{
		g_clear();
		for(i=0; i<N; ++i)
		{
			g_draw(points[i].x, points[i].y, points[i].color);
		}

		g_flush();
		g_msleep(20);
	}

	return 0;
}
