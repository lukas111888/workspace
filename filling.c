#include <stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{   
    system("rm -f /home/pi/workspace/test.tmp && sync && dd if=/dev/zero of=/home/pi/workspace/test.tmp bs=1K count=1 conv=fsync 2>&1 | grep -v records");
    system("rm -f /home/pi/workspace/test.tmp && sync && dd if=/dev/zero of=/home/pi/workspace/test.tmp bs=1M count=1 conv=fsync 2>&1 | grep -v records");
    system("rm -f /home/pi/workspace/test.tmp && sync && dd if=/dev/zero of=/home/pi/workspace/test.tmp bs=1G count=1 conv=fsync 2>&1 | grep -v records");
    system("rm -f /home/pi/workspace/test.tmp && sync && dd if=/dev/zero of=/home/pi/workspace/test.tmp bs=2 count=1 conv=fsync 2>&1 | grep -v records");
    system("rm -f /home/pi/workspace/test.tmp && sync && dd if=/dev/zero of=/home/pi/workspace/test.tmp bs=4 count=1 conv=fsync 2>&1 | grep -v records");
    return 0;
}