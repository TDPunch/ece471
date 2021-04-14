#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#include "write_display.h"

int init_display(void) {

	int fd = 0, result = 0, err = 0;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

	/* Open i2c device */

	fd = open(i2c_device, O_RDWR);
	if(fd < 0) {
		fprintf(stderr, "Error opening file.\n");
		err =  -16;
	}

	/* Set slave address */

	result = ioctl(fd, I2C_SLAVE, 0x70);
	if(result < 0) {
		fprintf(stderr, "Error with set slave address.\n");
		err = -17;
	}

	/* Turn on oscillator */

	buffer[0] = OSCILLATOR_ON;
	result = write(fd, buffer, 1);
	if(result != 1) {
		fprintf(stderr, "Error enabling oscillator.\n");
		err = -18;
	}

	/* Turn on Display, No Blink */

	buffer[0] = BLINKING_OFF;
	result = write(fd, buffer, 1);
	if(result != 1) {
		fprintf(stderr, "Error writing display setup register.\n");
		err = -19;
	}

	/* Set Brightness */

	buffer[0] = PW_12_16;
	result = write(fd, buffer, 1);
	if(result != 1) {
		fprintf(stderr, "Error changing brightness.\n");
		err = -20;
	}

	/* Write 1s to all Display Columns */

	buffer[0] = 0x0;
	for(int i = 0; i < 10; i+=2) {
		// Display pointer offset (Byte 0) should be 0 and Bytes 2,4,6
		// & 8 are not connected. Bytes 1,3,5,7,9 are display columns.
		buffer[i]   = 0x00;
		buffer[1+i] = 0xFF;
	}

	// Bytes 10-16 also not connected
	for(int i = 10; i < 17; i++) buffer[i] = 0x00;

	result = write(fd, buffer, 17);
	if(result != 17) {
		fprintf(stderr, "Error writing all 1's\n");
		err = -21;
	}

	// Return an error value if detected else all is well
	if(err < 0) return err;
	else {
		printf("\nDisplay Initialized.\n");
		return fd;
	}
}

int write_display(int fd, double value) {

	char buffer[17] = {0};
	int digits[4]  = {0};
	int result     = 0;
	int pick_case  = 0;

	// Check value to determine display protocol (case)
	pick_case = ((value >= 0.0) && (value <= 99.9)) ? 1 : pick_case;
	pick_case = ((value >= -99.9) && (value < 0.0)) ? 2 : pick_case;
	pick_case = ((value >= 100) && (value <= 999)) ? 3 : pick_case;

	// Function descriptions at bottom of file with definitions
	get_digits(digits, value, pick_case);
	get_7seg(digits);
	rmv_leading_zeros(digits);

	buffer[9] = DEGREE; // Regardless of case should always be degree

	switch (pick_case) {

		// Case 0.0 <= TEMP <= 99.9, display tens, ones, tenths
		case 1:
			buffer[7] = digits[3];
			buffer[3] = digits[2]|DECIMAL;
			buffer[1] = digits[1];
			break;

		// Case -99.9 <= TEMP < 0.0, display - and two digits
		case 2:
			// For values between -9.9 and 0.0 (with decimal)
			if(value > -10.0) {
				buffer[7] = digits[3];
				buffer[3] = digits[2]|DECIMAL;
			}
			// For values betweem -99.9 and -10.0 (no decimal)
			else {
				buffer[7] = digits[2];
				buffer[3] = digits[1];
			}
			buffer[1] = NEGATIVE; // Always show if negative
			break;

		// Case 100 <= TEMP <= 999
		case 3:
			buffer[7] = digits[2];
			buffer[3] = digits[1];
			buffer[1] = digits[0];
			break;

		// Case invalid temperature, indicate error (Err)
		default:
			buffer[9] = CLEAR;
			buffer[7] = CHAR_R;
			buffer[3] = CHAR_R;
			buffer[1] = CHAR_E;
			break;
	}

	// Write value to seven segment display
	result = write(fd, buffer, 17);
	if(result != 17) { // Error Checking
		fprintf(stderr, "Error writing to display\n");
		result = 1000;
		return result;
	}
	return value;
}

int shutdown_display(int fd) {

	char buffer[1] = {0};
	int result = 0;

	// Shut off the display
	buffer[0] = DISPLAY_OFF;
	result = write(fd, buffer, 1);

	// Error Checking
	if(result != 1) fprintf(stderr, "Error shutting off display.\n");

	// Close the device
	result = close(fd);

	// Error Checking
	if(result < 0) fprintf(stderr, "Error closing file.\n");
	else fprintf(stderr, "Shutdown Success!\n");
	return result;
}

/* Get digits of read value for display */
void get_digits(int *buf, double val, int round) {

	// Modulo operater won't function the same if negative
	if(val < 0.0) val *= -1.0;

	// Move decimal to get the digit
	int val_int = (int)(val * 10);

	// Get hundreds, tens, ones and tenths place
	for(int i = 3; i >= 0; i--) {
		buf[i] = val_int % 10;
		val_int /= 10;
	}

	// Round ones place based on case for accurate display
	if((round == 3) | (round == 2 && val > 10.0)) {
		buf[2] = (buf[3] < 5) ? buf[2] : (buf[2] + 1);
	}
}

/* Get bytes for 7 segment-display */
void get_7seg(int *buf) {
	char display_num[10] = {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};
	for(int i = 0; i < 10; i++) {
		if(buf[0] == i) buf[0] = display_num[i];
		if(buf[1] == i) buf[1] = display_num[i];
		if(buf[2] == i) buf[2] = display_num[i];
		if(buf[3] == i) buf[3] = display_num[i];
	}
}

/* Clear leading 0's */
void rmv_leading_zeros(int *buf) {
	if(buf[0] == (ZERO)) {
		buf[0] = CLEAR;
		if(buf[1] == (ZERO)) buf[1] = CLEAR;
	}
}
