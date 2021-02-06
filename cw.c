#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char **argv) {
        struct timeval tv, tvt;
        int diff;
        gettimeofday(&tv, 0);
        diff = tvt.tv_usec - tv.tv_usec;
        if (fork() != 0) {
                gettimeofday(&tvt, 0);
                diff = tvt.tv_usec - tv.tv_usec;
                printf("Context switch time %d usec\n", diff);
        }
        return 0;
}