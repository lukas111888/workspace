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
    printf("Press CTRL-C to exit.\n");
    while (!signal_received) {
        digitalWrite(redLED, HIGH);
        delay(500);
        digitalWrite(redLED, LOW);
        delay(500);
    }
    pinMode(redLED, INPUT);
    printf("\nCompleted cleanup of GPIO resources.\n");
    return(signal_received);
}