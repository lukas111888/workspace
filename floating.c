#include <stdio.h>
#include <sys/time.h> 
int main(){    
    struct timeval start,end; 
    double result;
    float a=0, b=3.3; 
    gettimeofday(&start, NULL );
    // float type has 1 bit symbol, 8 bit exponent, 23 decimal
    //Range from -3.40E+38 to +3.40E+38

    //Addition    
    for(int i=0;i<1000000000;i++){
        a+=0.01;
        }   
    //Subtraction
    for(int i=0;i<1000000000;i++){
        a-=0.01;
        }  
    //Multiplication
    for(int i=0;i<1000000000;i++){
        b =b*1.1;
        //printf("%f\n",b);
    }
    //Division
    for(int i=0;i<1000000000;i++){
        b = b/1.1;
        }
    gettimeofday(&end, NULL );
    long timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    result = timeuse /1000000.0;
    printf("Floating Arithmetic benchmark used %fsec\n",result);

    return 0;
}