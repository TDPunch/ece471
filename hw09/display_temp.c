#include <stdio.h>
#include <unistd.h>

#include "read_temp.h"
#include "write_display.h"

int main(int argc, char **argv) {

	double temp = 0;
	int fd = 0, err = 0, i = 0;

	// Initialize display and check for errors
	fd = init_display();
	if(fd < -15) printf("Error Initializing.\n");
	sleep(1);

	while(i < 10) {
		// Read temperature and check for errors
		temp = read_temp();
		if(temp < -100) printf("Error invalid temp.\n");


		// Write temperature to display and check for errors
		err = write_display(fd, temp);
		if(err > 999) printf("Error writing to display.\n");

		sleep(1);	// Wait for 1 second
		i++;		// Count to 10 to test shutdown
	}

	// Shutdown display and chack for errors
	err = shutdown_display(fd);
	if(err < 0) printf("Error in shutdown");

	return 0;
}

