#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/mman.h>

#include "graphic.h"

#define N 200

typedef struct {
  int x,y,color;
} Point;

int main() {
  Point points[N];
  int i;

  // /* semaphore POSIX */
  // int sem;
  // /* segment de mémoire partagée contenant le tableau des N points */
  // int *shm;
  // /* initialisation du tableau de points */
  // if((shm=(int*)shm_open("/shmem", O_RDWR|O_CREAT|O_EXCL, 0600)) == -1)
  // {
  //   perror("shm_open"), exit(1);
  // }
  // /* affectation de la taille (fichier creux) */
  // if(ftruncate(shm, sizeof(int)))
  // {
  //   perror("fseek"), exit(1);
  // }

  /* initialisation aléatoire des N points */

  for(i=0; i<N; ++i) {
    points[i].x=g_random()%(G_WIDTH-200)+100;
    points[i].y=g_random()%(G_HEIGHT-200)+100;
    points[i].color=g_random()%G_NB_COLORS;
  }

  // /* pour i allant de 0 à N-1 créer un fils */
  // for(i=0;i<N;i++)
  // {
  //   switch(fork())
  //   {
  //     case -1 : perror("fork"); exit(1);
  //     case  0 : *shm=1234; exit(0);
  //   }
  // }

  g_init();

  while(1) {
    /* mise à jour des points */
    for(i=0; i<N; ++i) {
      points[i].x+=g_random()%2?1:-1;
      points[i].y+=g_random()%2?1:-1;
    }

    /* Affichage des points */
    g_clear();
    for(i=0; i<N; ++i)
      g_draw(points[i].x, points[i].y, points[i].color);

    g_flush();
    g_msleep(20);
  }

  return 0;
}
