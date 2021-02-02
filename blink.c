#include <signal.h>
#include <stdio.h>
#include <wiringPi.h>
#define redLED 0
volatile sig_atomic_t signal_received = 0;
void sigint_handler(int signal) {
    signal_received = signal;
}
int main(void) {
    signal(SIGINT, sigint_handler);
    wiringPiSetup();
    pinMode(redLED, OUTPUT);
    printf("\nSignalValue:   %d\n",signal_received);
    printf("Press CTRL-C to exit.\n");
    while (signal_received != 2) {
        digitalWrite(redLED, HIGH);
        delay(500);
        digitalWrite(redLED, LOW);
        delay(500);
    }
    printf("\nSignalValue:   %d\n",signal_received);
    pinMode(redLED, INPUT);
    printf("\nCompleted cleanup of GPIO resources.\n");
    return(signal_received);
}