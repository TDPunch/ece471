#include <stdio.h>
#include <stdlib.h>	/* atof() */
#include <unistd.h>

#include "write_display.h"

int main(int argc, char **argv) {

	int fd;

	// Create test cases for cases a, a with leading zeroes,
	// b > -10, b < -10, c and all cases that could require,
	// rounding. Also error case.
	double test1=72.9, test2=-10.5, test3=-10.4, test4=-9.4;
	double test5=-100.0, test6=4.9, test7=471.5, test8=471.4;

	while(1) {
		// Test init function
		fd = init_display();

		// Test write_display() function
		write_display(fd, test1);
		printf("Testing temperature with %.4lf\n",test1);
		sleep(2);
		write_display(fd, test2);
		printf("Testing temperature with %.4lf\n",test2);
		sleep(2);
		write_display(fd, test3);
		printf("Testing temperature with %.4lf\n",test3);
		sleep(2);
		write_display(fd, test4);
		printf("Testing temperature with %.4lf\n",test4);
		sleep(2);
		write_display(fd, test5);
		printf("Testing temperature with %.4lf\n",test5);
		sleep(2);
		write_display(fd, test6);
		printf("Testing temperature with %.4lf\n",test6);
		sleep(2);
		write_display(fd, test7);
		printf("Testing temperature with %.4lf\n",test7);
		sleep(2);
		write_display(fd, test8);
		printf("Testing temperature with %.4lf\n",test8);
		sleep(2);

		// Test shutdown function
		shutdown_display(fd);
	}
	return 0;
}

