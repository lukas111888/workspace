#include <wiringPi.h>
#include <stdio.h>
#include <signal.h>
#define BCM3 RPI_GPIO_P1_5 
volatile sig_atomic_t signal_received = 0;
void sigint_handler(int signal) {
    signal_received = signal;
}

void lightLED(void) {
static int x = 1; // store number of times pressed. Use static
// to retain the state on multiple calls
digitalWrite(LED_GPIO, HIGH); // turn the LED on
cout << "Button pressed " << x++ << " times! LED on" << endl;
}

int main(void)
{
    signal(SIGINT, sigint_handler);
    if (!bcm2835_init())
      return 1;
 
    bcm2835_gpio_fsel(BCM3, BCM2835_GPIO_FSEL_INTP);
    printf("Press CTRL-C to exit.\n");
    while (signal_received != 2) 
    {
        
    }
    bcm2835_close();
    printf("\n");
    return 0;
}