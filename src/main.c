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
#include "gui.h"
#include "gps.h"
#include "handle_googleapi_key.h"

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

    gui_open();

    pthread_create(&gps_thread,NULL,gps_receiver_thread,NULL);
    pthread_join(gps_thread);

    gui_close();
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
    
    result = createUrl_FromLatLon(lat,lon,GOOGLE_API_KEY,url);

    if(result == true){
	http_retrieve(url,http_callback);

    } else {
	printf("createUrl_FromLatLon failed\n");
    }

}

void gps_receiver_thread(void* arg)
{
    struct gps_params param;

    memset(&param,0,sizeof(param));
    param.callback = gps_callback;

    gpsRetriever("/dev/ttyUSB0",&param);
}

void usage(void)
{
    printf("maptracer [tty device for GPS module]\n");
}

