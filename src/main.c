/*
  Simple map data retriever for Google Static Maps API V2

  Version 5/1/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "gui.h"
#include "gps.h"
#include "handle_googleapi_key.h"

void gps_receiver_thread(void* arg);
void gui_thread(void *arg);
void usage(void);


int main(int argc,char** argv)
{
    pthread_t gps_th;
    pthread_t gui_th;
    int opt;
    bool cvbs_resolution = false;
    bool demo_mode = false;
    bool no_gui_mode = false;
    unsigned char tty_dev[64];

    if(argc == 1){
	usage();
	return -1;
    }
    printf("MapTracer is launched\n");

    XInitThreads();

    while((opt = getopt(argc,argv,"p:cnd")) != -1){
	switch(opt){
	case 'c':
	    cvbs_resolution = true;
	    printf("cvbs\n");
	    break;
	case 'p':
	    strcpy(tty_dev,optarg);
	    break;
	case 'n':
	    printf("No GUI Mode(debug)\n");
	    no_gui_mode = true;
	    break;
	case 'd':
	    demo_mode = true;
	    break;
	default:
	  usage();
	    break;
	}
    }

    gpsOpen(tty_dev,demo_mode);
    gui_open(cvbs_resolution,no_gui_mode);

    pthread_create(&gps_th,NULL,gps_receiver_thread,NULL);
    pthread_create(&gui_th,NULL,gui_thread,NULL);

    pthread_join(gps_th);
    pthread_join(gui_th);

    gui_close();
    gpsClose();

}

void gui_thread(void *arg)
{
    gui_EventHandler();

    printf("\nGUI THREAD TERMINATED\n");

    gpsStopRetriever();
}

void gps_receiver_thread(void* arg)
{
    struct gps_callback_params param;

    memset(&param,0,sizeof(param));
    param.callback = NULL;

    gpsStartRetriever(&param);
}

void usage(void)
{
    printf("maptracer [tty device for GPS module] [option]\n");
    printf(" Copyright(C) 2013 Yasuhiro ISHII\n\n");
    printf(" [option]\n");
    printf("  -c        : CVBS mode(low resolution mode)\n");
    printf("  -n        : No GUI mode(for debug)\n");
    printf("  -p [tty]  : Specify tty device\n");
    printf("  -d        : DEMO Mode\n");
}

