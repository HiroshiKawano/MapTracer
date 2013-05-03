/*
  Simple map tracer for RaspberryPi
  Version 5/3/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#include <stdio.h>
#include <stdbool.h>

#define DEBUG

bool createUrl_FromLatLon(double lat,double lon,unsigned char* api_key,unsigned char* url)
{
    int zoom = 18;
    int size_x = 600;
    int size_y = 600;
    bool sensor = true;
    
    if(url == NULL){
	return(false);
    }
    if(api_key == NULL){
	return(false);
    }

    sprintf(url,
	    "http://maps.googleapis.com/maps/api/staticmap?center=%f,%f&zoom=%d&size=%dx%d&sensor=%s&key=%s",
	    lat,
	    lon,
	    zoom,
	    size_x,
	    size_y,
	    sensor == true ? "true" : "false",
	    api_key);
    
#ifdef DEBUG
    printf("%s :[%s]\n",__func__,url);
#endif
    
    return(true);
}
