#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <wiringPi.h> 
#include <gps.h>


//define bool type

#define bool int


//warning light distance setup in cm

#define limitWarning 8


//***********GPIO MAP*******************
// GPIO 24 for left ultrasonic sensor trigger
// GPIO 25 for left ultrasonic sensor echo detection
// GPIO 20 for right ultrasonic sensor trigger
// GPIO 21 for right ultrasonic sensor echo detection
// GPIO 13 for left turning sign detection
// GPIO 19 for right turning sign detection
// GPIO 26 for reverse sign detection
// GPIO 27 for left warning light
// GPIO 22 for right warning light
// GPIO  0 for turn on/off left side camera
// GPIO  5 for turn on/off right side camera
// GPIO  6 for turn on/off reverse camera 
// GPIO 26 for detection for vehicle on/off 

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
#define poweroff 26

 
/*
 * GPIO setup
 *********************************************************************************
 */

void setup() {     
    wiringPiSetupGpio();  // initialize the device for WiringPi
    gps_init(); // initialize the device for GPS

// Setup pin mode for GPIO
    pinMode(signL, INPUT); 
    pinMode(signR, INPUT);     
    pinMode(signB, INPUT); 
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
    pinMode(poweroff, INPUT); 
    pullUpDnControl (poweroff, PUD_DOWN);

    usleep(10000);
}


/*
 * power off interrupt function for raspberry pi 4
 * safely halt device after vehicle turn off
 * save vehicle battery from drain
 *********************************************************************************
 */

void turnOff(void) 
{
    usleep(2000);// Eliminate fluctuate

    if (digitalRead(poweroff) == LOW)
    {
        printf("sudo halt\n");
        system("sudo halt");
    }
}


/*
 * left/right turning sign detection interrupt function
 * switch vedio output to screen
 * screen off by default
 *********************************************************************************
 */

void turnLRB(void) 
{
    usleep(2000);//Eliminate fluctuate

// check only one turning sign is on
// otherwise, no output shows on screen

    if (digitalRead(signL) != digitalRead(signR))
    {
        digitalWrite(vedioL, digitalRead(signL));
        digitalWrite(vedioR, digitalRead(signR));
        
    }
    else 
    {
        digitalWrite(vedioL, HIGH);
        digitalWrite(vedioR, HIGH);

    } 
}


/*
 * thread function for GPS
 * GPS update every 1 second
 *********************************************************************************
 */

void *threadGPS(void *recordGPS)
{
    loc_t *record=(loc_t *)recordGPS; // gps data structure for record
    loc_t gps; // gps data structure for temp
    while(1)
    {
        gps_location(&gps); // read gps data
        if (gps.latitude != 0 && gps.longitude != 0 ) // determine data is value
        {
            *record = gps; //copy temp gps value to record
        }
        printf("The RPi location is (%lf,%lf)\n", gps.latitude, gps.longitude);
        printf("http://maps.google.com/maps?q=%lf,%lf\n",record->latitude, record->longitude); //print location on the map
        sleep(1);
    }
    return 0;
}


/*
 * thread function for left side warning LED light
 * LED light on when object in the warning distance
 *********************************************************************************
 */

void *threadLEFTwarning(void *warning)
{
    bool *leftwarning=(bool *)warning;
    clock_t tempT;
    struct timeval startT, endT;
    int diffT;
    int distance=0;

    while(1)
    {
//Send trig pulse (activate sign)
 
        digitalWrite(leftTRIG, HIGH);
        usleep(50);
        digitalWrite(leftTRIG, LOW);

// Wait for echo start

        while (digitalRead(leftECHO) == LOW);
        gettimeofday(&startT, 0); // record start time
        

// Wait for echo end in RANGE 400CM

        tempT=clock();
        while ((digitalRead(leftECHO) == HIGH) && (clock()-tempT)/1000<22);
        gettimeofday(&endT, 0); // record end time
    
// calculate distance in cm

        diffT=1000000 * ( endT.tv_sec - startT.tv_sec ) + endT.tv_usec - startT.tv_usec;
        distance = diffT*0.017;

//light on/off depend on distance

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

        usleep(100000);  // repeat distance measurement every 100000 micro sec
    }    
    return leftwarning;
}


/*
 * thread function for right side warning LED light
 * LED light on when object in the warning distance
 *********************************************************************************
 */

void *threadRIGHTwarning(void *warning)
{
    bool *rightwarning=(bool *)warning;
    clock_t tempT;
    struct timeval startT, endT;
    int diffT;
    int distance=0;

    while(1)
    {
//Send trig pulse (activate sign)
 
        digitalWrite(rightTRIG, HIGH);
        usleep(50);
        digitalWrite(rightTRIG, LOW);

// Wait for echo start
        while (digitalRead(rightECHO) == LOW);
        gettimeofday(&startT, 0);// record start time
        

// Wait for echo end in RANGE 400CM
        tempT=clock();
        while ((digitalRead(rightECHO) == HIGH) && (clock()-tempT)/1000<22);
        gettimeofday(&endT, 0);// record end time
    
// calculate distance in cm

        diffT=1000000 * ( endT.tv_sec - startT.tv_sec ) + endT.tv_usec - startT.tv_usec;
        distance = diffT*0.017;

//light on/off depend on distance

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

        usleep(100000); // repeat distance measurement every 100000 micro sec      
    }    
    return rightwarning;
}


/*
 * main function
 *********************************************************************************
 */

int main(void) { 
    bool leftwarning=FALSE,rightwarning=FALSE;
    loc_t recordGPS;

    setup();

// active interrupt for turn sign and power off

    wiringPiISR(signL, INT_EDGE_BOTH, &turnLRB);
    wiringPiISR(signR, INT_EDGE_BOTH, &turnLRB);
    wiringPiISR(poweroff, INT_EDGE_FALLING, &turnOff);

// create the threads, pass the reference, address of the function and data
// pthread_create() returns 0 on the successful creation of a thread
 
    pthread_t thread0; // thread0 for left LED
    if(pthread_create(&thread0, NULL, &threadLEFTwarning, &leftwarning)!=0)
    {
        printf("Failed to create the thread" );
        return 1;
    }

    pthread_t thread1; // thread1 for right LED
    if(pthread_create(&thread1, NULL, &threadRIGHTwarning, &rightwarning)!=0)
    {
        printf("Failed to create the thread" );
        return 1;
    }

    pthread_t thread2;// thread2 for GPS 
    if(pthread_create(&thread2, NULL, &threadGPS, &recordGPS)!=0)
    {
        printf("Failed to create the thread" );
        return 1;
    }

// loop for main function. dispaly some info

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