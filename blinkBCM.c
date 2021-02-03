#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#define BCM11 RPI_GPIO_P1_23 
volatile sig_atomic_t signal_received = 0;
void sigint_handler(int signal) {
    signal_received = signal;
}
int main(void)
{
    signal(SIGINT, sigint_handler);
    if (!bcm2835_init())
      return 1;
 
    bcm2835_gpio_fsel(BCM11, BCM2835_GPIO_FSEL_OUTP);
    printf("Press CTRL-C to exit.\n");
    while (signal_received != 2) 
    {
        bcm2835_gpio_write(BCM11, HIGH);
        bcm2835_delay(500);
        bcm2835_gpio_write(BCM11, LOW);
        bcm2835_delay(500);
    }
    bcm2835_close();
    printf("\n");
    return 0;
}