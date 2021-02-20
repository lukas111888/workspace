#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

#define LED_GPIO 17 // this is GPIO17, Pin 11
#define BUTTON_GPIO 3 // this is GPIO27, Pin 13
// the Interrupt Service Routine (ISR) to light the LED
void lightLED(void) {
static int x = 1; // store number of times pressed. Use static
// to retain the state on multiple calls
digitalWrite(LED_GPIO, HIGH); // turn the LED on
printf("Button pressed %d times\n",x++ );
}
int main() { // must be run as root
wiringPiSetupGpio();a // use the GPIO numbering
pinMode(LED_GPIO, OUTPUT); // the LED
pinMode(BUTTON_GPIO, INPUT); // the Button
digitalWrite (LED_GPIO, LOW); // LED is off
// call the lightLED() ISR on the rising edge (i.e., button press)
wiringPiISR(BUTTON_GPIO, INT_EDGE_RISING, &lightLED);
for(int i=20; i>0; i--) { // countdown to program end
sleep(1); // sleep for 1 second
}
return 0; // program ends after 10s
}