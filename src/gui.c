/*
  Simple map tracer for RaspberryPi
  Version 5/1/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "gui.h"

//#define DEBUG

bool gui_open(void);
bool gui_close(void);
bool showPngFileOnMemory_FullScreen(void* pngdata,int size);
bool showPngFile_FullScreen(unsigned char *filename);

#if 0
int main(void)
{
    FILE* fp;
    unsigned char* buff;
    int size;
    
    gui_open();
    
    buff=malloc(1024*1024);
    
    fp = fopen("../a.png","rb");
    size=fread(buff,1,1024*1024,fp);
    printf("file read:%d byte(s)\n",size);
    fclose(fp);
    
    showPngFileOnMemory_FullScreen(buff,size);
    
    sleep(1);
    
    free(buff);
    gui_close();
    
    return(0);
}
#endif

bool gui_open(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
	printf("SDL_Init failed (%s)\n",SDL_GetError());
	return false;
    }
    return true;
}

bool gui_close(void)
{
    SDL_Quit();
    return true;
}

bool showPngFileOnMemory_FullScreen(void* pngdata,int size)
{
    SDL_Surface* screen = NULL;
    SDL_Surface* image = NULL;
    SDL_RWops *rw;
    const SDL_VideoInfo *videoInfo = NULL;
    
    videoInfo = SDL_GetVideoInfo();
    if(videoInfo == 0){
	printf("SDL_GetVideoInfo failed (%s)\n",SDL_GetError());
    return(false);
    }
    
    rw = SDL_RWFromMem(pngdata,size);
    
    image = (SDL_Surface*)IMG_LoadPNG_RW(rw,0);
    if(!image){
	printf("IMG_Load failed (%s)\n",SDL_GetError());
	return(-1);
    }
  
    screen = SDL_SetVideoMode(image->w,image->h,videoInfo->vfmt->BitsPerPixel,SDL_HWSURFACE);
    if(!screen){
	printf("SDL_SetVideoMode failed (%s)\n",SDL_GetError());
	return(false);
    }
    
    SDL_BlitSurface(image,0,screen,0);
    SDL_FreeSurface(image);
    
    return(true);
}

bool showPngFile_FullScreen(unsigned char *filename)
{
    SDL_Surface* screen = NULL;
    SDL_Surface* image = NULL;
    const SDL_VideoInfo *videoInfo = NULL;
    
    videoInfo = SDL_GetVideoInfo();
    if(videoInfo == 0){
	printf("SDL_GetVideoInfo failed (%s)\n",SDL_GetError());
	return(false);
    }
    
    image = (SDL_Surface*)IMG_Load(filename);
    if(!image){
	printf("IMG_Load failed (%s)\n",SDL_GetError());
	return(false);
    }
    
    screen = SDL_SetVideoMode(image->w,image->h,videoInfo->vfmt->BitsPerPixel,SDL_HWSURFACE);
    if(!screen){
	printf("SDL_SetVideoMode failed (%s)\n",SDL_GetError());
	return(false);
    }
  
    SDL_BlitSurface(image,0,screen,0);
    SDL_FreeSurface(image);
    
    return(true);
}
