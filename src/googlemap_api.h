/*
  Simple map tracer for RaspberryPi
  Version 5/3/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#ifndef __GOOGLEMAP_API_H__
#define __GOOGLEMAP_API_H__

bool googlemaps_createUrl_FromLatLon(double lat,double lon,unsigned char* api_key,unsigned char* url);
bool googlestreetview_createUrl_FromLatLon(double lat,double lon,int heading,unsigned char* api_key,unsigned char* url);

#endif /* __GOOGLEMAP_API_H__ */
