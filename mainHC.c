#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h> 
 
#define bool int
#define TRUE 1
#define FALSE 0

#define leftTRIG 24
#define leftECHO 25
#define rightTRIG 20
#define rightECHO 21
#define limitWarning 8

 
void setup() { 
    
      
    bcm2835_gpio_fsel(leftTRIG, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(leftECHO, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(rightTRIG, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(rightECHO, BCM2835_GPIO_FSEL_INPT);
    //bcm2835_gpio_set_pud(leftECHO, BCM2835_GPIO_PUD_DOWN);
 
    //TRIG pin must start LOW
    bcm2835_gpio_write(rightTRIG, LOW);
    bcm2835_gpio_write(leftTRIG, LOW);
    usleep(10000);


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
 
        bcm2835_gpio_write(leftTRIG, HIGH);
        usleep(50);
        bcm2835_gpio_write(leftTRIG, LOW);

        //Wait for echo start
        while (bcm2835_gpio_lev(leftECHO) == LOW);
        gettimeofday(&startT, 0);
        tempT=clock();

        //Wait for echo end MAX RANGE 2cm to 400CM
        while ((bcm2835_gpio_lev(leftECHO) == HIGH) && (clock()-tempT)/1000<22);
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
 
        bcm2835_gpio_write(rightTRIG, HIGH);
        usleep(50);
        bcm2835_gpio_write(rightTRIG, LOW);

        //Wait for echo start
        while (bcm2835_gpio_lev(rightECHO) == LOW);
        gettimeofday(&startT, 0);
        tempT=clock();

        //Wait for echo end MAX RANGE 2cm to 400CM
        while ((bcm2835_gpio_lev(rightECHO) == HIGH) && (clock()-tempT)/1000<22);
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
    //    bcm2835_set_debug(1);
    if (!bcm2835_init())
	return 1;
    bool leftwarning=FALSE,rightwarning=FALSE;
    setup();


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