#ifndef PERSO_H
#define PERSO_H

#include "video.h"

typedef struct {
	Video *video;
	char name[100];
} Perso;

void init_perso();

#endif


