#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <wiringPi.h> 
#include <gps.h>

 
#define bool int
#define limitWarning 8

//GPIO MAP
#define leftTRIG 24
#define leftECHO 25
#define rightTRIG 20
#define rightECHO 21
#define signL 13
#define signR 19
#define signB 26
#define ledL 27
#define ledR 22
#define vedioL 0
#define vedioR 5
#define vedioB 6


 

void setup() { 
    
    wiringPiSetupGpio();   
    //pullUpDnControl (signL, PUD_UP);
    pinMode(signL, INPUT); 
    //pullUpDnControl (signR, PUD_UP);
    pinMode(signR, INPUT); 
    
    pinMode(signB, INPUT); 
    //pullUpDnControl (signB, PUD_DOWN);
    pinMode(ledL, OUTPUT);
    pinMode(ledR, OUTPUT);
    pinMode(vedioL, OUTPUT);
    pinMode(vedioR, OUTPUT);
    pinMode(vedioB, OUTPUT);
    digitalWrite(ledL, LOW);
    digitalWrite(ledR, LOW);
    digitalWrite(vedioL, HIGH);
    digitalWrite(vedioR, HIGH);


    pinMode(leftTRIG, OUTPUT);
    pinMode(leftECHO, INPUT);
    pinMode(rightTRIG, OUTPUT);
    pinMode(rightECHO, INPUT);
    digitalWrite(rightTRIG, LOW);
    digitalWrite(leftTRIG, LOW);

    gps_init(); // initialize the device



    usleep(10000);


}

void *threadGPS(void *recordGPS)
{
    loc_t *record=(loc_t *)recordGPS;
    loc_t gps; // a location structure
    while(1)
    {
        gps_location(&gps); // determine the location data
        if (gps.latitude != 0 && gps.longitude != 0 )
        {
            *record = gps;
        }
        printf("The RPi location is (%lf,%lf)\n", gps.latitude, gps.longitude);
        printf("http://maps.google.com/maps?q=%lf,%lf\n",record->latitude, record->longitude);
        sleep(1);
    }
    return 0;
}

void turnLRB(void) 
{
    usleep(2000);//Eliminate fluctuate
    if (digitalRead(signL) != digitalRead(signR))
    {
        digitalWrite(vedioL, digitalRead(signL));
        digitalWrite(vedioR, digitalRead(signR));
        
    }

    else 
    {
        sleep(0.8);
        digitalWrite(vedioL, HIGH);
        digitalWrite(vedioR, HIGH);

    }

    

}

void *threadLEFTwarning(void *warning)
{
    bool *leftwarning=(bool *)warning;

    clock_t tempT;
    struct timeval startT, endT;
    int diffT;
    int distance=0;
    while(1)
    {
        //detect min error range less than 2cm
            
        //Send trig pulse (activate sign)
 
        digitalWrite(leftTRIG, HIGH);
        usleep(50);
        digitalWrite(leftTRIG, LOW);

        //Wait for echo start
        while (digitalRead(leftECHO) == LOW);
        gettimeofday(&startT, 0);
        tempT=clock();

        //Wait for echo end MAX RANGE 2cm to 400CM
        while ((digitalRead(leftECHO) == HIGH) && (clock()-tempT)/1000<22);
        gettimeofday(&endT, 0);
    
        //Get distance in cm
        diffT=1000000 * ( endT.tv_sec - startT.tv_sec ) + endT.tv_usec - startT.tv_usec;
        distance = diffT*0.017;
        //printf("%d, and Distance: %dcm\n",diffT, distance);
        if (distance>limitWarning)
        {
            *leftwarning=FALSE;
            digitalWrite(ledL, LOW);
        }
        else 
        {            
            *leftwarning=TRUE;
            digitalWrite(ledL, HIGH);
        }             
        usleep(100000);       
    }    
    return leftwarning;
}


void *threadRIGHTwarning(void *warning)
{
    bool *rightwarning=(bool *)warning;

    clock_t tempT;
    struct timeval startT, endT;
    int diffT;
    int distance=0;
    while(1)
    {
        //detect min error range less than 2cm
            
        //Send trig pulse (activate sign)
 
        digitalWrite(rightTRIG, HIGH);
        usleep(50);
        digitalWrite(rightTRIG, LOW);

        //Wait for echo start
        while (digitalRead(rightECHO) == LOW);
        gettimeofday(&startT, 0);
        tempT=clock();

        //Wait for echo end MAX RANGE 2cm to 400CM
        while ((digitalRead(rightECHO) == HIGH) && (clock()-tempT)/1000<22);
        gettimeofday(&endT, 0);
    
        //Get distance in cm
        diffT=1000000 * ( endT.tv_sec - startT.tv_sec ) + endT.tv_usec - startT.tv_usec;
        distance = diffT*0.017;
        //printf("%d, and Distance: %dcm\n",diffT, distance);
        if (distance>limitWarning)
        {
            *rightwarning=FALSE;
            digitalWrite(ledR, LOW);
        }
        else 
        {            
            *rightwarning=TRUE;
            digitalWrite(ledR, HIGH);
        }             
        usleep(100000);       
    }    
    return rightwarning;
}

 
int main(void) {

 
    bool leftwarning=FALSE,rightwarning=FALSE;
    loc_t recordGPS;
    setup();

    wiringPiISR(signL, INT_EDGE_BOTH, &turnLRB);
    wiringPiISR(signR, INT_EDGE_BOTH, &turnLRB);

    // create the threads, pass the reference, address of the function and data
    // pthread_create() returns 0 on the successful creation of a thread
 
    pthread_t thread0;
    if(pthread_create(&thread0, NULL, &threadLEFTwarning, &leftwarning)!=0){
        printf("Failed to create the thread" );
        return 1;
    }

    pthread_t thread1;
    if(pthread_create(&thread1, NULL, &threadRIGHTwarning, &rightwarning)!=0){
        printf("Failed to create the thread" );
        return 1;
    }

    pthread_t thread2;
    if(pthread_create(&thread2, NULL, &threadGPS, &recordGPS)!=0){
        printf("Failed to create the thread" );
        return 1;
    }


    while(1)
    {
        
        if (leftwarning ==TRUE)
        {
            printf("left side warning\n");
        }
        if (rightwarning ==TRUE)
        {
            printf("right side warning\n");
            
        }
        if (digitalRead(signL)==HIGH)
        {
            printf("Left side camera is ON\n");
        }
        if (digitalRead(signR)==HIGH)
        {
            printf("Right side camera is ON\n");
        }
        usleep(100000);

    }
    
    
 
    return 0;
}