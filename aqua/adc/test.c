#include <stdio.h>

#include <pigpio.h>


/*
MOSI=10 MISO=9  SCLK=10 CE0=8  CE1=7              standard SPI gpios

mosi=20 miso=19 sclk=21 ce0=18 ce1=17 ce2=16      aux SPI gpios (B+ only)
*/

int main(int argc, char *argv[])
{
   int dp_out, dp_in, adc, p, pval, aval;

   char rxBuf[3], txBuf[3];

   if (gpioInitialise() < 0) return 1;


   /*
      Get handle to digital pot to send command byte and setting
      Can write at 2 Mbps
   */

   dp_out = spiOpen(1, 2000000, 0);

   if (dp_out < 0) return 2;

   /*
      Get handle to digital pot to send command byte and receive reading
      Can only read at 250kbps
   */

   dp_in = spiOpen(1, 250000, PI_SPI_FLAGS_3WIRE(1)|PI_SPI_FLAGS_3WREN(1));
   if (dp_in < 0) return 3;

   /*
      Get handle to aux SPI channel 1
      Can read at 2 Mbps
   */

   adc = spiOpen(1, 2000000, PI_SPI_FLAGS_AUX_SPI(1));
   if (adc < 0) return 4;

   for (p=0; p<129; p++)
   {
      txBuf[0] = 0x06 ;
      txBuf[1] = p;
      spiXfer(dp_out, txBuf, NULL, 2); /* set pot to p */

      txBuf[0] = 0x0C;
      txBuf[1] = 0;
      spiXfer(dp_in, txBuf, rxBuf, 2); /* read pot position */
      pval = rxBuf[1];


      txBuf[0] = 1;
      txBuf[1] = 0xC0; /* read channel 2 of ADC */
      txBuf[2] = 0;

      spiXfer(adc, txBuf, rxBuf, 3); /* read ADC */
      aval = (rxBuf[1]*256)+rxBuf[2];

      printf("pot=%3d adc=%4d\n", pval, aval);
   }

   spiClose(adc);
   spiClose(dp_in);
   spiClose(dp_out);

   gpioTerminate();
}
