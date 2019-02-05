#include <stdio.h>
#include <unistd.h>

#include "perso.h"
#include "editeur.h"

int main(int argc, char** argv) {

	init_video();
	init_perso();

	while(1) {
		printf("edit\n");
		sleep(1);
	}

	return 0;
}
