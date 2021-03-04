#include <stdio.h>
#include <stdlib.h>
#include <gps.h>
//#define PORTNAME "/dev/ttyAMA1"
int main(void) {

    //printf("%s\n",PORTNAME);
    gps_init(); // initialize the device
    loc_t gps,record; // a location structure
    while(1)
    {
        gps_location(&gps); // determine the location data
        if (gps.latitude != 0 && gps.longitude != 0 )
        {
            record = gps;

        }
        printf("The RPi location is (%lf,%lf)\n", gps.latitude, gps.longitude);
        printf("http://maps.google.com/maps?q=%lf,%lf\n",record.latitude, record.longitude);
        //printf("Altitude: %lf m. Speed: %lf knots\n", gps.altitude, gps.speed);
    }
    return 0;
}