#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
 
//#define TRUE 1
 
#define TRIG 5
#define ECHO 6
 
void setup() {
        wiringPiSetup();
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
 
        //TRIG pin must start LOW
        digitalWrite(TRIG, LOW);
        delay(30);
}
 
float getCM() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);
 
        //Wait for echo start
        while(digitalRead(ECHO) == LOW);
 
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;
        //diffT=1000000 * ( endT.tv_sec - startT.tv_sec ) + endT.tv_usec - startT.tv_usec;
        //printf("%ld\n",travelTime);
        //Get distance in cm
        float distance = (float)travelTime *0.017;
 
        return distance;
}
 
int main(void) {
        setup();
        while(1)
        {
                printf("Distance: %.2fcm\n", getCM());
                delay(500);
        }
 
        return 0;
}