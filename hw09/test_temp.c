#include <stdio.h>
#include <stdlib.h>	/* for exit() */
#include <unistd.h>	/* for sleep() */

#include "read_temp.h"

int main(int argc, char **argv) {

	double current_temp;

	while(1) {
		current_temp=read_temp();
		if (current_temp<-1000.0) {
			fprintf(stderr,"Error reading temperature!\n");
			exit(-1);
		}

		printf("Current temperature is %.2f degrees C\n",current_temp);

		sleep(1);
	}

	return 0;
}
