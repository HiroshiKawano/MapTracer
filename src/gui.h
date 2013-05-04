/*
  Simple map tracer for RaspberryPi
  Version 5/1/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#ifndef __GUI_H__
#define __GUI_H__

bool gui_open(bool cvbs_resolution);
bool gui_close(void);
void gui_EventHandler(void);
bool showPngFileOnMemory_FullScreen(void* pngdata,int size);
bool showPngFile_FullScreen(unsigned char *filename);

#endif /* __GUI_H__ */

