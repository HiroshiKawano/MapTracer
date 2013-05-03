#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gui.h"

void gps_receiver_thread(void* arg);
void usage(void);

int main(int argc,char** argv)
{
    pthread_t gps_thread;

    printf("MapTracer is launched\n");

    if(argc != 2){
	usage();
	return -1;
    }

    pthread_create(&gps_thread,NULL,gps_receiver_thread,NULL);

    pthread_join(gps_thread);
}

void gps_receiver_thread(void* arg)
{
    while(1){
	printf("Hello\n");
	sleep(1);
    }
}

void usage(void)
{
    printf("maptracer [tty device for GPS module]\n");
}
