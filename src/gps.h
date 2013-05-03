/*
  Simple map tracer for RaspberryPi
  Version 5/4/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#ifndef __GPS_H__
#define __GPS_H__

struct gps_params {
  void (*callback)(double lat,double lon);
};

extern bool gpsRetriever(char* interface,struct gps_params* params);

#endif /* __GPS_H__ */
