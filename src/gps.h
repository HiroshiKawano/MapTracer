/*
  Simple map tracer for RaspberryPi
  Version 5/4/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#ifndef __GPS_H__
#define __GPS_H__

struct gpsParams {
    double lat;
    double lon;
    int sig;
    int fix;
    int elv;
    double dir;
    int sat_inview;
    int sat_inuse;
};

struct gps_callback_params {
  void (*callback)(double lat,double lon);
};

extern bool gpsStartRetriever(struct gps_callback_params* params);
extern bool gpsStopRetriever(void);
extern bool gpsOpen(char* interface,bool demo);
extern bool gpsClose(void);
extern bool gpsGetCurrentParams(struct gpsParams* p);

#endif /* __GPS_H__ */
