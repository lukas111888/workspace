#include <stdio.h>
#include <sys/time.h> 
int main(){    
    struct timeval start,end; 
    double result;
    int a=0, b=1; 
    gettimeofday(&start, NULL );
    // int type range from -2147483648  to 2147483647

    //Addition    
    while(a != 2147483647){
        a+=1;
        }   

    //Subtraction
    while(a != -2147483648){
        a-=1;
        }  

    //Multiplication
    while(b >= 2147483647){
        b =b*2;
    }

    //Division
    while(b < 1){
        b = b/2;
        }
    gettimeofday(&end, NULL );
    long timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    result = timeuse /1000000.0;
    printf("Integer Arithmetic benchmark used %fsec\n",result);

    return 0;
}