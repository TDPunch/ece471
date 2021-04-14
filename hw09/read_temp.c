#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "read_temp.h"

double read_temp(void) {

	double result=0.0;

	char crc[3], temp[7]; // Expect max 3 bytes for YES; Max 7 bytes for t=XXXXX

	FILE *therm;

	therm = fopen(SENSOR_NAME, "r"); // Open thermometer
	if(therm < 0) {			 // Error checking
		fprintf(stderr, "Error opening sensor\n");
		return -101;
	}

	// Read 8 bytes and CRC. Only care about YES/NO identifier
	// and temp in mili-degrees at the end of the lines
	// 12th byte and 10th byte respectively.
	fscanf(therm, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", crc, temp);

	// Check to make sure CRC is valid
	if(strcmp(crc, "YES")) {	// Error checking
		fprintf(stderr, "Did not pass CRC, invalid\n");
		return -102;
	}

	// Format is "t=temp" skip the t= part and convert from milidegrees
	// to degrees celsius
	result = (atoi(temp + 2)) / 1000.0;
	result = (result * (9.0/5.0) + 32); // Convert to fahrenheit

	// Reset char arrays
	memset(crc, 0, sizeof(crc));
	memset(temp, 0, sizeof(temp));

	return result;
}
