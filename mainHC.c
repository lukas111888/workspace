#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <pthread.h>
 
#define bool int
#define TRUE 1
#define FALSE 0

#define leftTRIG 24
#define leftECHO 25


 
void setup() { 
    
      
    bcm2835_gpio_fsel(leftTRIG, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(leftECHO, BCM2835_GPIO_FSEL_INPT);
    //bcm2835_gpio_set_pud(leftECHO, BCM2835_GPIO_PUD_DOWN);
 
    //TRIG pin must start LOW
    bcm2835_gpio_write(leftTRIG, LOW);
    usleep(10000);


}
 
float getCM(int T, int E) {
    int TRIG=T, ECHO=E;

    struct timeval startT, endT;
    int diffT;
    
    
    //detect min error range less than 2cm
    while (1)
    {
        //Send trig pulse (activate sign)
        bcm2835_gpio_write(TRIG, HIGH);
        usleep(30);
        bcm2835_gpio_write(TRIG, LOW);

        //Wait for echo start
        while (bcm2835_gpio_lev(ECHO) == LOW);
        gettimeofday(&startT, 0);

        //Wait for echo end
        while (bcm2835_gpio_lev(ECHO) == HIGH);
        gettimeofday(&endT, 0);
    
        //Get distance in cm
        diffT = endT.tv_usec-startT.tv_usec;
        if (diffT>120)
        break;
    }
    float distance = diffT*0.017; 
    return distance;
}

void *threadLEFTwarning(void *warning)
{
    bool *leftwarning=(bool *)warning;
    while(1)
    {
        float distance = getCM(leftTRIG,leftECHO);
        //printf("Distance: %.2fcm\n", distance);
        usleep(100000);
        if(distance >8)
        {
            *leftwarning=FALSE;
        }
        else
        {
            *leftwarning=TRUE;
        }        
    }    
    return leftwarning;
}
 
int main(void) {
    //    bcm2835_set_debug(1);
    if (!bcm2835_init())
	return 1;
    bool leftwarning=FALSE,righWarning=FALSE;
    setup();


    // create the threads, pass the reference, address of the function and data
    // pthread_create() returns 0 on the successful creation of a thread
    pthread_t thread0;
    if(pthread_create(&thread0, NULL, &threadLEFTwarning, &leftwarning)!=0){
        printf("Failed to create the thread" );
        return 1;
    }


    while(1)
    {
        if (leftwarning ==TRUE)
        {
            printf("left side warning\n");
            usleep(10000); // 延迟关闭
        }
        /**
        else
        {
            printf("no warning   %d\n",leftwarning);
        }
        **/
        usleep(100000);
    }
    
    
 
    return 0;
}