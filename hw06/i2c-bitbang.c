#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#include "gpio.h"
#include "i2c-bitbang.h"

/* Global variables */
static int scl_fd,sda_fd;

/* Pull the SDA gpio pin low */
static int SDA_gpio_pull_low(void) {

	int result=0;

	/* This function should set the SDA GPIO direction to out */
	/* and then write the value '0' to sda_fd		  */

	gpio_set_output(SDA_GPIO);	// Set GPIO2 (SDA) direction to write
	result = write(sda_fd, "0", 1);	// Write 0 to SDA
	if(result < 0) {		// Error checking
		fprintf(stderr, "Error writing in SDA_gpio_pull_low\n");
		return 0;
	}

	/* return the result of the write() call */

	return result;

}

/* Pull the SCL gpio pin low */
static int SCL_gpio_pull_low(void) {

	int result=0;

	/* This function should set the GPIO direction to write */
	/* and then write the value '0' to sda_fd		*/

	gpio_set_output(SCL_GPIO);	// Set GPIO3 (SCL) direction to write
	result = write(scl_fd, "0", 1); // Write 0 to SCL
	if(result < 0) {		// Error checking
		fprintf(stderr, "Error writing in SCL_gpio_pull_low\n");
		return 0;
	}

	/* return the result of the write() call */

	return result;

}

/* Let SDA float high */
static int SDA_gpio_float_high(void) {

	int result=0;

	/* This function should set the SDA GPIO direction to input	*/
	/* Which allows the open collector bus to float high		*/

	gpio_set_input(SDA_GPIO);	// Set GPIO2 (SDA) direction to read

	return result;

}

/* Let SCL float high */
static int SCL_gpio_float_high(void) {

	int result=0;

	/* This function should set the SCL GPIO direction to input	*/
	/* Which allows the open collector bus to float high		*/

	gpio_set_input(SCL_GPIO);	// Set GPIO3 (SCL) direction to read

	return result;

}

/* read the value of SDA */
static int read_SDA(void) {

	unsigned char value = 0;
	unsigned char buffer[1] = "0";	// Buffer for reading ASCII from file
	int result = 0;			// Result variable for error checking

	/* This function should set the GPIO direction to read	*/
	/* rewind the file descriptor, then read SDA.		*/
	/* remember to convert from ASCII before returning.	*/

	gpio_set_input(SDA_GPIO);	// Set GPIO2 (SDA) direction to read
	lseek(sda_fd, 0, SEEK_SET);	// Rewind sda_fd before reading
	result = read(sda_fd, buffer, 1);
	if(result < 0) {		// Error Checking
		fprintf(stderr, "Error reading SDA\n");
		return result;
	}
	value = (int)buffer;		// Type cast to decimal

	return value;
}

/* read the value of SCL */
static int read_SCL(void) {

	unsigned char value = 0;
	unsigned char buffer[1] = "0";	// Buffer for reading ASCII from file
	int result = 0;			// Result variable for error checking

	/* This function should set the GPIO direction to read	*/
	/* rewind the file descriptor, then read SCL		*/
	/* remember to convert from ASCII before returning.	*/

	gpio_set_input(SCL_GPIO);	// Set GPIO3 (SCL) direction to read
	lseek(scl_fd, 0, SEEK_SET);	// Rewind scl_fd before reading
	result = read(scl_fd, buffer, 1);
	if(result < 0) {
		fprintf(stderr, "Error reading SCL\n");
		return result;
	}
	value = (int)buffer;		// Type cast to decimal

	return value;
}



static int I2C_delay(void) {

	usleep(4);	/* 4us, although due to sysfs overhead */
			/* and inherent usleep() limitations   */
			/* we are going to be much slower.     */

	return 0;

}


static void i2c_start_bit(void) {

	/* For a start bit, SDA goes from high to low while SCL is high */

	SCL_gpio_float_high();
	I2C_delay();
	SDA_gpio_float_high();
	SDA_gpio_pull_low();
	I2C_delay();
	SCL_gpio_pull_low();

	return;
}


static void i2c_stop_bit(void) {

	/* For a stop bit, SDA goes from low to high while SCL is high */

	SCL_gpio_float_high();
	I2C_delay();
	SDA_gpio_pull_low();
	SDA_gpio_float_high();
	I2C_delay();
	SCL_gpio_pull_low();

	return;
}

static int i2c_read_bit(void) {

	int bit=0;

	/* Let SDA and SCL go high.  Then read SDA.  The set SCL low */

	SDA_gpio_float_high();	// Let SDA go high and
	I2C_delay();		// wait a delay
	SCL_gpio_float_high();	// Let SCL go high and
	I2C_delay();		// wait a delay

	bit = read_SDA();	// Read SDA
	I2C_delay();		// Delay then
	SCL_gpio_pull_low();	// pull SCL low

	return bit;

}

static int i2c_write_bit(int bit) {

	/* Pull SCL low.  Set sda to the value you want. */
	/* Then toggle SCL high then low */

	SCL_gpio_pull_low();
	if(bit) SDA_gpio_float_high();
	else SDA_gpio_pull_low();
	I2C_delay();
	SCL_gpio_float_high();
	I2C_delay();
	SCL_gpio_pull_low();

	return 0;

}

	/* Write 8 bits out and check for NACK */
static int i2c_write_byte(unsigned char byte) {

	int nack=1;

	// Loop writes all 8 bits starting with the MSB
	for(int i = 7; i >= 0; i--) {
		i2c_write_bit((byte >> i) & 0x01);
	}

	nack = i2c_read_bit();	// Read nack bit

	return nack;
}


	/* Write a series of bytes */
int write_i2c(unsigned char *bytes, int number) {

	int i;
	int result=0;

	i2c_start_bit();

	for(i=0;i<number;i++) {
		result=i2c_write_byte(bytes[i]);
		if (result<0) {
			printf("Error writing!\n");
			break;
		}

	}

	i2c_stop_bit();

	return result;

}


/* Initialize two file descriptors to be used for SDA and SCL */
/* They are global variables to make the code a bit easier    */
int i2c_init_gpios(int sda, int scl) {

        char string[BUFSIZ];

	/* Enable the GPIOs */
	if (gpio_enable(sda)<0) printf("Error enabling SDA\n");
	if (gpio_enable(scl)<0) printf("Error enabling SCL\n");

	/* Open the SDA fd */
	printf("Using GPIO%d as SDA\n",sda);
        sprintf(string,"/sys/class/gpio/gpio%d/value",sda);
        sda_fd=open(string,O_RDWR);
        if (sda_fd<0) {
                fprintf(stderr,"\tError getting value!\n");
                return -1;
        }

	/* Open the SCL fd */
	printf("Using GPIO%d as SCL\n",scl);
	sprintf(string,"/sys/class/gpio/gpio%d/value",scl);

        scl_fd=open(string,O_RDWR);
        if (scl_fd<0) {
                fprintf(stderr,"\tError getting value!\n");
                return -1;
        }

	/* Set the read/write direction of the GPIOs */
	gpio_set_input(sda);
	gpio_set_input(scl);

	return 0;
}
