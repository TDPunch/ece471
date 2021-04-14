#include <stdio.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>


int main(int argc, char **argv) {

	int fcheck = open("/sys/class/gpio/gpio18/value", O_WRONLY);
	char buffer[10];
	int fe, fd, fv;

	// Check if GPIO 18 has already been exported
	if(fcheck < 0) {
		// Open the esport file to export GPIO pin 18
		// and check if the file has been opened.
		fe = open("/sys/class/gpio/export", O_WRONLY);
		if(fe < 0) fprintf(stderr, "\tError enabling\n");
		strcpy(buffer, "18");	// No ASCII for 18 so copy to char buffer
		write(fe, buffer, 2);	// Write the buffer to the file
		close(fe);		// Close the file
		usleep(1000000);  	// Wait 1sec for the export process
	}
	close(fcheck); // Close fcheck

	// Open direction file and check if it opened, then configure GPIO18
	// as an output to blink the LED then close the file.
	fd = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
	if(fd < 0) {
		// If file didn't open print an error message and exit
		fprintf(stderr, "\tError!\n");
		return 0;
	}
	write(fd, "out", 3);
	close(fd);

	// Open value file and check if it opened, then in an infinite loop
	// set the value high or low every .5 sec.
	fv = open("/sys/class/gpio/gpio18/value", O_WRONLY);
	if(fv < 0) {
		// If file didn't open print an error message an exit
		fprintf(stderr, "\tError!\n");
		return 0;
	}
	while(1) {
		// The following will blink the LED in a morse code message
		// one time unit will be considered .5 sec. The rules of IMC
		// are: dots are one time unit, dash is three time units,
		// space between parts of the same letter is one time unit,
		// space between letters is three time units, and the space between
		// words is seven time units

		// Message is: ECE471
		write(fv, "1", 2); // E: dot
		usleep(500000);

		write(fv, "0", 2); // Space between letters
		usleep(1500000);

		write(fv, "1", 2); // C: dash dot dash dot
		usleep(1500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(1500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);

		write(fv, "0", 2); // Space between letters
		usleep(1500000);

		write(fv, "1", 2); // E: dot
		usleep(500000);

		write(fv, "0", 2); // Space between letters
		usleep(1500000);

		write(fv, "1", 2); // 4: dot dot dot dot dash
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(1500000);

		write(fv, "0", 2); // Space between characters
		usleep(1500000);

		write(fv, "1", 2); // 7: dash dash dot dot dot
		usleep(1500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(1500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(500000);

		write(fv, "0", 2); // Space between characters
		usleep(1500000);

		write(fv, "1", 2); // 1: dot dash dash dash dash
		usleep(500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(1500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(1500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(1500000);
		write(fv, "0", 2);
		usleep(500000);
		write(fv, "1", 2);
		usleep(1500000);

		write(fv, "0", 2); // End of message wait 5 sec before repeating
		usleep(5000000);

	}
	close(fv);

	return 0;

}
