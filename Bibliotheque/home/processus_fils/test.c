#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "graphic.h"

#define N 200

//LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/graphic/usr/lib
//export LD_LIBRARY_PATH


typedef struct
{
	int x,y,color;
} Point;

void codeFils(sem_t *semaphore, Point points[],int i)
{
	if(sem_wait(semaphore) == -1)
  {
    perror("sem_wait"), exit(1);
  }
  points[i].x+=g_random()%2?1:-1;
  points[i].y+=g_random()%2?1:-1;
  if(sem_post(semaphore) == -1)
  {
    perror("sem_post"), exit(1);
  }
}

int main()
{
	Point points[N];
	int i;

	/* semaphore POSIX */
	sem_t *semaphore;


	for(i=0; i<N; i++)
  {
    if((semaphore=sem_open("/sem1", O_CREAT|O_EXCL, 0666, 0)) == SEM_FAILED)
    {
      perror("sem_open");
      exit(1);
    }
  }

  /* segment de mémoire partagée contenant le tableau des N points */
  int shmfd; int *shmaddr;

  if((shmfd=shm_open("/shmem", O_RDWR|O_CREAT|O_EXCL, 0600)) == -1 )
  {
    perror("shm_open");
    exit(1);
  }

  /* le fichier doit avoir la taille requise */
  if(ftruncate(shmfd, sizeof(int))) perror("fseek"), exit(1);

  if((shmaddr=(int *)mmap(0, sizeof(int),PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0)) == MAP_FAILED)
  {
    perror("mmap");
    exit(1);
  }

	/* initialisation aléatoire des N points */

	for(i=0; i<N; ++i)
  {
		points[i].x=g_random()%(G_WIDTH-200)+100;
		points[i].y=g_random()%(G_HEIGHT-200)+100;
		points[i].color=g_random()%G_NB_COLORS;
	}

	g_init();

	while(1)
  {
  	/* mise à jour des points */
  	for(i=0; i<N; ++i)
    {
  	   switch(fork())
       {
         case -1 : perror("fork"); exit(1);
         case  0 : /**shmaddr=points;*/ codeFils(semaphore, points,i); exit(0);
       }
  	}

    /* Affichage des points */
  	g_clear();
  	for(i=0; i<N; ++i)
    {
      g_draw(points[i].x, points[i].y, points[i].color);
    }

  	g_flush();
  	g_msleep(20);
	}

	 if(munmap(shmaddr, sizeof(int)))
   {
     perror("unmap");
     exit(1);
   }
   if(shm_unlink("/shmem") == -1)
   {
     perror("unlink");
     exit(1);
   }

   return 0;
}
