#include <stdio.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>


int main(int argc, char **argv) {

	char buffer[10], read_buf[1];	// Buffers for reading and writing
	int fcheck, fe, fd, fv;		// Variables for file manipulation
	int count, switch_state;	// Variables for debouncing in the loop

	// If GPIO pin 17 has already been exported this file will exist.
	// If it doesn't exist then export GPIO pin 17
	fcheck = open("/sys/class/gpio/gpio17/value", O_RDONLY);
	if(fcheck < 0) {
		fe = open("/sys/class/gpio/export", O_WRONLY);
		// Check if file has been opened, if not print error
		if(fe < 0) {
			fprintf(stderr, "\tError Enabling!\n");
			return 0;
		}
		strcpy(buffer, "17");	// Convert 17 to ACII 1 and 7
		write(fe, buffer, 2);	// Write "17" to file to export
		close(fe);		// Close file
		usleep(50000);		// Wait 50ms for export process
	}
	close(fcheck);	// Close check file

	fd = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
	// Check if file has been opened, if not print error
	if(fd < 0) {
		fprintf(stderr, "\tError!\n");
		return 0;
	}
	write(fd, "in", 2);	// Set GPIO pin 17 as input
	close(fd);		// Close file

	// Initialize debouncing variables to 0.
	count = 0;
	switch_state = 0;

	// Infinitly read value from GPIO17
	while(1) {
		fv = open("/sys/class/gpio/gpio17/value", O_RDONLY);
		if(fv < 0) {
			// Check if file has been opened, if not print error
			fprintf(stderr, "\tError!\n");
			return 0;
		}
		read(fv, read_buf, 16); // Read value

		// Software debouncing using counting method
		if(switch_state == 1 && read_buf[0] == 48) {
			// Reset counter and switch_state and print to screen
			count = 0;
			switch_state = 0;
			printf("Switch is released.\n");
		}
		else if(read_buf[0] == 49) count++;	// Increment the counter
		if(count == 5000) {
			// Set state to high and print to screen
			switch_state = 1;
			printf("Switch is pressed.\n");
		}
		close(fv);
	}

	return 0;

}
