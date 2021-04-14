#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

// Display setup register commands
#define BLINKING_OFF (0x8<<4)|(0x1) // Binary 0b10000001, blinking off
#define BLINKING_2HZ (0x8<<4)|(0x3) // Binary 0b10000011, blink at 2HZ
#define BLINKING_1HZ (0x8<<4)|(0x5) // Binary 0b10000101, blink at 1HZ
#define BLINKING_HALFHZ (0x8<<4)|(0x7) // Binary 0b10000111, blink at 0.5Hz

// Digital Dimming Data Input
#define PW_1SIXTEENTH (0x7<<4)|(0x0)
#define PW_2SIXTEENTH (0x7<<4)|(0x1)
#define PW_3SIXTEENTH (0x7<<4)|(0x2)
#define PW_4SIXTEENTH (0x7<<4)|(0x3)
#define PW_5SIXTEENTH (0x7<<4)|(0x4)
#define PW_6SIXTEENTH (0x7<<4)|(0x5)
#define PW_7SIXTEENTH (0x7<<4)|(0x6)
#define PW_8SIXTEENTH (0x7<<4)|(0x7)
#define PW_9SIXTEENTH (0x7<<4)|(0x8)
#define PW_10SIXTEENTH (0x7<<4)|(0x9)
#define PW_11SIXTEENTH (0x7<<4)|(0xA)
#define PW_12SIXTEENTH (0x7<<4)|(0xB)
#define PW_13SIXTEENTH (0x7<<4)|(0xC)
#define PW_14SIXTEENTH (0x7<<4)|(0xD)
#define PW_15SIXTEENTH (0x7<<4)|(0xE)
#define PW_16SIXTEENTH (0x7<<4)|(0xF)

void ece(unsigned char buf[], int f);
void class_num(unsigned char buf[], int f);

int main(int argc, char **argv) {

	int fd, result;

//	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

	/* Open i2c device */

	fd = open("/dev/i2c-1", O_RDWR);
	if(fd < 0) fprintf(stderr, "Error opening file.\n");

	/* Set slave address */

	result = ioctl(fd, I2C_SLAVE, 0x70);
	if(result < 0) fprintf(stderr, "Error with set slave address.\n");

	/* Turn on oscillator */

	buffer[0] = (0x2 << 4) | (0x1);
	result = write(fd, buffer, 1);
	if(result != 1) fprintf(stderr, "Error enabling oscillator.\n");

	/* Turn on Display, No Blink */

	buffer[0] = BLINKING_OFF;
	result = write(fd, buffer, 1);
	if(result != 1) fprintf(stderr, "Error writing display setup register.\n");

	/* Set Brightness */

	buffer[0] = PW_12SIXTEENTH;
	result = write(fd, buffer, 1);
	if(result != 1) fprintf(stderr, "Error changing brightness.\n");

	/* Write 1s to all Display Columns */

	buffer[0] = 0x0;
	for(int i = 0; i < 16; i++) buffer[1+i] = 0xFF;
	write(fd, buffer, 17);

	/* Display ECE 471 in infinite loop */
	while(1) {
		ece(buffer, fd); // Display ECE
		usleep(1000000); // Pause for 1s
		class_num(buffer, fd); // Display 471
		usleep(1000000); // Pause for 1s
	}

	/* Close device */

	close(fd);

	return 0;
}

void ece(unsigned char buf[], int f) {

	buf[0] = 0x00; // Display pointer offset
	buf[1] = 0x79; // Column 1, Segments ADEFG ("E")
	buf[2] = 0x00; // Next 8 bits of column 1 not connected
	buf[3] = 0x39; // Column 2, Segments ADEF  ("C")
	buf[4] = 0x00; // Next 8 bits of column 2 not connected
	buf[5] = 0x00; // Column 3, empty
	buf[6] = 0x00; // Next 8 bits of column 3 not connected
	buf[7] = 0x79; // Column 4, segments ADEFG ("E")
	buf[8] = 0x00; // Next 8 bits of column 4 not connected
	buf[9] = 0x00; // Column 5, empty
	// Bytes 10-16 empty
	for(int i = 10; i < 17; i++) buf[i] = 0x00;

	write(f, buf, 17);

}

void class_num(unsigned char buf[], int f) {

	buf[0] = 0x00; // Display pointer offset
	buf[1] = 0x66; // Column 1, Segments BCFG ("4")
	buf[2] = 0x00; // Next 8 bits of column 1 not connected
	buf[3] = 0x07; // Column 2, Segments ABC ("7")
	buf[4] = 0x00; // Next 8 bits of column 2 not connected
	buf[5] = 0x00; // Column 3, empty
	buf[6] = 0x00; // Next 8 bits of column 3 not connected
	buf[7] = 0x06; // Column 4, segments BC ("1")
	buf[8] = 0x00; // Next 8 bits of column 4 not connected
	buf[9] = 0x00; // Column 5, empty
	// Bytes 10-16 empty
	for(int i = 10; i < 17; i++) buf[i] = 0x00;

	write(f, buf, 17);

}
