#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <wiringPi.h> 

 
#define bool int

#define leftTRIG 24
#define leftECHO 25
#define rightTRIG 20
#define rightECHO 21
#define limitWarning 8
#define BUTTON 3

 
void setup() { 
    
    wiringPiSetupGpio();
    pinMode(BUTTON, INPUT); 
    pinMode(leftTRIG, OUTPUT);
    pinMode(leftECHO, INPUT);
    pinMode(rightTRIG, OUTPUT);
    pinMode(rightECHO, INPUT);


    digitalWrite(rightTRIG, LOW);
    digitalWrite(leftTRIG, LOW);


    usleep(10000);


}


void lightLED(void) 
{
    static int x = 1; // store number of times pressed. Use static
    // to retain the state on multiple calls
    printf("Button pressed %d  times! LED on\n",x++);
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
        }
        else 
        {            
            *leftwarning=TRUE;
            usleep(1000000); // 延迟关闭
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
        }
        else 
        {            
            *rightwarning=TRUE;
            usleep(1000000); // 延迟关闭
        }             
        usleep(100000);       
    }    
    return rightwarning;
}

 
int main(void) {
 
    bool leftwarning=FALSE,rightwarning=FALSE;
    setup();

    wiringPiISR(BUTTON, INT_EDGE_RISING, &lightLED);


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
        usleep(100000); // 延迟关闭
        /**
        else
        {
            printf("no warning   %d\n",leftwarning);
        }
        **/
        //usleep(100000);
    }
    
    
 
    return 0;
}