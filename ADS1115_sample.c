// ADS1115_sample.c - 12/9/2013. Written by David Purdie as part of the openlabtools initiative
// Initiates and reads a single sample from the ADS1115 (without error handling)


#include <stdio.h>
#include <fcntl.h>		// open
#include <inttypes.h>		// uint8_t/etc.
#include <linux/i2c-dev.h>	// I2C bus definitions.

int main()
{
	static int ADS_address = 0x48;		// Address of the device on the I2C bus.
	int I2CFile;

	uint8_t writeBuf[3];			// Buffer to store the 3 bytes that we write to the I2C device.
	uint8_t readBuf[2];			// 2 byte buffer to store the data read from the I2C device.

	int16_t val;				// Store the 16 bit value of our ADC conversation.
	float voltsToFile;

        if( getuid() != 0 )
	{
		printf("Please use sudo user!\n");
		return -1;
	}

	I2CFile = open("/dev/i2c-2", O_RDWR);	// Open the I2C device.
	ioctl(I2CFile, I2C_SLAVE, ADS_address);	// Specify the address of the I2C Slave to communicate with.

	// These three bytes are written to the ADS1115 to set the config register and start a conversation.
	writeBuf[0] = 1;		// This sets the pointer register so that the following two bytes write to the configuration register.
	writeBuf[1] = 0xC0;		// This sets the 8 MSBs of the config reg. (bits 15-8) to 11010010.
	writeBuf[2] = 0x83;		// This sets the 8 LSBs of the config reg. (bits 7-0) to 00000011.

	// Initialize the buffer used to read data from the ADS1115 to 0
	readBuf[0] = 0;
	readBuf[1] = 0;

	// Write writeBuf to the ADS1115, the 3 specifies the number of bytes we are writing.
	// This begins a single conversion.
	write(I2CFile, writeBuf, 3);

	writeBuf[0] = 0;			// Set pointer register to 0 to read from the conversion reg.
	write(I2CFile, writeBuf, 1);

	int i;
 	for(i=0;i<100;i++)
	{
		read(I2CFile, readBuf, 2);		// Read the contents of the conversion register into readBuf.
		val = readBuf[0] << 8 | readBuf[1];	// Combine the two bytes of readBuf into a single 16 bit result.
		voltsToFile = (double)val*6.144/32767.0;
		printf( "Current volts: %f (V)\n", voltsToFile );
		sleep(2);
	}
	close(I2CFile); //Close fd
	return 0; // Success
}
