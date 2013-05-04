/*
  Simple map tracer for RaspberryPi
  Version 5/1/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#ifndef __GUI_H__
#define __GUI_H__

extern bool gui_open(bool ,bool );
extern bool gui_close(void);
extern void gui_EventHandler(void);
extern bool showImageFileOnMemory_FullScreen(void* data,int size,int format);
extern bool showPngFile_FullScreen(unsigned char *filename);

enum ImageFormat {
  ImageFormat_JPEG,
  ImageFormat_PNG
};

#endif /* __GUI_H__ */

