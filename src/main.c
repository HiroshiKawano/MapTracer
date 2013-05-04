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
    unsigned char tty_dev[64];

    printf("MapTracer is launched\n");

    if((argc == 2)||(argc == 3)){
	usage();
	return -1;
    }

    XInitThreads();

    while((opt = getopt(argc,argv,"p:c")) != -1){
	switch(opt){
	case 'c':
	    cvbs_resolution = true;
	    printf("cvbs\n");
	    break;
	case 'p':
	    strcpy(tty_dev,optarg);
	    break;
	default:
	    break;
	}
    }

    gpsOpen(tty_dev);
    gui_open(cvbs_resolution);

    pthread_create(&gps_th,NULL,gps_receiver_thread,NULL);
    pthread_create(&gui_th,NULL,gui_thread,NULL);

    pthread_join(gps_th);
    pthread_join(gui_th);

    gui_close();
    gpsClose();

}

void http_callback(void* buff,int len)
{
  //printf("%s Len=%d\n",__func__,len);

  showPngFileOnMemory_FullScreen(buff,len);
  usleep(500000);
}

void gps_callback(double lat,double lon)
{
    unsigned char url[1024];
    bool result;

#if 0
    /* for test,below is the point that Osaka-station */
    lat = 34.702385;
    lon = 135.495195;
#endif

    result = createUrl_FromLatLon(lat,lon,GOOGLE_API_KEY,url);

    if(result == true){
	http_retrieve(url,http_callback);

    } else {
	printf("createUrl_FromLatLon failed\n");
    }

}

void gui_thread(void *arg)
{
    gui_EventHandler();

    printf("Terminate gui thread");

    gpsStopRetriever();
}

void gps_receiver_thread(void* arg)
{
    struct gps_params param;

    memset(&param,0,sizeof(param));
    param.callback = gps_callback;

    gpsStartRetriever(&param);
}

void usage(void)
{
    printf("maptracer [tty device for GPS module] [option]\n");
    printf(" Copyright(C) 2013 Yasuhiro ISHII\n\n");
    printf(" [option]\n");
    printf("  -c        : CVBS mode(low resolution mode)\n");
}

