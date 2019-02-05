#include <stdio.h>
#include <unistd.h>

#include "video.h"
#include "perso.h"
#include "warcraft.h"

int main(int argc, char** argv) {

	init_video();
	init_perso();

	while(1) {
		printf("play\n");
		sleep(1);
	}

	return 0;
}
