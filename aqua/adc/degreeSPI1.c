#include <stdio.h>
#include <wiringPi.h>
#include <pigpio.h>



#define CS_MCP3204 1

/*
   MOSI=10 MISO=9  SCLK=10 CE0=8  CE1=7              standard SPI gpios

   mosi=20 miso=19 sclk=21 ce0=18 ce1=17 ce2=16      aux SPI gpios (B+ only)
 */


int main(int argc, char *argv[])
{
	int dp_out, dp_in, adc, p, pval, aval;
	int adcValue=0, adcChannel=0;


	unsigned char txBuf[3],rxBuf[3];

	if (gpioInitialise() < 0) return 1;


	/*
	   Get handle to digital pot to send command byte and setting
	   Can write at 2 Mbps
	 */





	adc = spiOpen(1, 2000000, PI_SPI_FLAGS_AUX_SPI(1));
	if (adc < 0) return 4;


	if(wiringPiSetup() == -1)
		return 1;
	pinMode(CS_MCP3204,OUTPUT);	
	

		
	while(1)
	{
	

		txBuf[0] = 0x06 | ((adcChannel & 0x07) >> 2);
		txBuf[1] = ((adcChannel & 0x07) << 6);
		txBuf[2] = 0x00;

		digitalWrite(CS_MCP3204,0); //LOW : CS Active

		spiXfer(adc, txBuf, rxBuf, 3); /* set pot to p */


		txBuf[1] = 0x0F & txBuf[1];


		txBuf[1] = 0x0f & txBuf[1];
		adcValue = (rxBuf[1]<<8)|rxBuf[2];

		digitalWrite(CS_MCP3204,1); //HIGH : CS Inactive

		printf("adc0 Value = %u\n",adcValue);

		
		delay(1000);

	}




	spiClose(adc);

	gpioTerminate();
}
