#include <stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{   
    system("sysbench --num-threads=4 --validate=on --test=memory --memory-block-size=1K --memory-total-size=1K --memory-access-mode=seq run | grep 'Operations\|transferred\|total time:\|min:\|avg:\|max:' | tr -s [:space:]");
    system("sysbench --num-threads=4 --validate=on --test=memory --memory-block-size=1K --memory-total-size=1M --memory-access-mode=seq run | grep 'Operations\|transferred\|total time:\|min:\|avg:\|max:' | tr -s [:space:]");
    system("sysbench --num-threads=4 --validate=on --test=memory --memory-block-size=1K --memory-total-size=1G --memory-access-mode=seq run | grep 'Operations\|transferred\|total time:\|min:\|avg:\|max:' | tr -s [:space:]");
    system("sysbench --num-threads=4 --validate=on --test=memory --memory-block-size=1K --memory-total-size=2B --memory-access-mode=seq run | grep 'Operations\|transferred\|total time:\|min:\|avg:\|max:' | tr -s [:space:]");
    system("sysbench --num-threads=4 --validate=on --test=memory --memory-block-size=1K --memory-total-size=4B --memory-access-mode=seq run | grep 'Operations\|transferred\|total time:\|min:\|avg:\|max:' | tr -s [:space:]");

    return 0;
}