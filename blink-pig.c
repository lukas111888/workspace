#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
int main(int argc, char *argv[])
{
    if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }


    gpioSetMode(11, PI_OUTPUT);    
    while (1)
    {
	    // Turn it on
	    gpioWrite(11, 1);	
	    // wait a bit
	    time_sleep(0.5);	
	    // turn it off
	    gpioWrite(11, 0);	
	    // wait a bit
	    time_sleep(0.5);
    }
    return 0;
}