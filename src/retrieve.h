/*
  Simple map data retriever for Google Static Maps API V2

  Version 5/1/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#ifndef __RETRIEVE_H__
#define __RETRIEVE_H__

bool http_retrieve(unsigned char* url,bool (*callback)(void* buff,int len));

#endif /* __RETRIEVE_H__ */
