#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3

int main(int argc, char **argv) {

        int spi_fd;
	int mode = SPI_MODE_0;
	int lsb_mode = 0;
	int result = 0;
	int value = 0;
	float vin = 0;
	struct spi_ioc_transfer spi;
	unsigned char data_out[LENGTH] = {0x1, 0x2, 0x3};
	unsigned char data_in[LENGTH];

	/* Open SPI device */

	spi_fd = open("/dev/spidev0.0", O_RDWR);  // Open for read/write access
	if(spi_fd < 0) {
		fprintf(stderr, "Error opening\n");
		return 0;
	}

	/* Set SPI Mode_0 */

	result = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	if(result < 0) {
		fprintf(stderr, "Error setting SPI mode\n");
		return 0;
	}

	/* Set bit order to LSB first*/

	result = ioctl(spi_fd, SPI_IOC_WR_LSB_FIRST, &lsb_mode);
	if(result < 0) {
		fprintf(stderr, "Error setting LSB first\n");
		return 0;
	}

	/* Kernel doesn't like stray values */
	memset(&spi, 0, sizeof(struct spi_ioc_transfer));

	// Setup full-duplex transfer of 3 bytes
	spi.tx_buf = (unsigned long)&data_out;
	spi.rx_buf = (unsigned long)&data_in;
	spi.len    = LENGTH;
	spi.delay_usecs   = 0;
	spi.speed_hz      = 100000;
	spi.bits_per_word = 8;
	spi.cs_change     = 0;

	/* Loop forever printing the CH0 and CH1 Voltages 	*/
	/* Once per second.					*/

	while(1) {

		// Transmit 3 bytes to read from CH0
		data_out[2] = 0x00;
		data_out[1] = 0x80; // Read CH0
		data_out[0] = 0x01; // Start bit

		// Run one full-duplex transaction
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		if(result < 0) {
			fprintf(stderr, "Error reading from CH0\n");
			return 0;
		}

		// Put 10 bits received into an integer
		value = ((data_in[1] & 0x3) << 8) + data_in[2];
		vin = value * 3.3/1024.0; // Convert to floating point voltage
		printf("CH0 = %f\n", vin);

		// Transmit 3 bytes to read from CH1
		data_out[2] = 0x00;
		data_out[1] = 0x90; // Read CH1
		data_out[0] = 0x01; // Start bit

		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		if(result < 0) {
			fprintf(stderr, "Error reading from CH1\n");
			return 0;
		}

		// Put 10 bits recieved into an integer
		value = ((data_in[1] & 0x3) << 8) + data_in[2];
		vin = value * 3.3/1024.0; // Convert to floating point voltage
		printf("CH1 = %f\n", vin);

		usleep(1000000); // Delay one second
	}

	return 0;
}
