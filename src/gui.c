/*
  Simple map tracer for RaspberryPi
  Version 5/4/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "gui.h"

//#define DEBUG

bool gui_open(bool cvbs);
bool gui_close(void);
bool showPngFileOnMemory_FullScreen(void* pngdata,int size);
bool showPngFile_FullScreen(unsigned char *filename);

static bool cvbs_resolution = false;

bool gui_open(bool cvbs)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
	printf("SDL_Init failed (%s)\n",SDL_GetError());
	return false;
    }
    cvbs_resolution = cvbs;
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
  
    if(cvbs_resolution == false){
      screen = SDL_SetVideoMode(image->w,
				image->h,
				videoInfo->vfmt->BitsPerPixel,
				SDL_HWSURFACE);
    } else {
      screen = SDL_SetVideoMode(image->w,
				400,
				videoInfo->vfmt->BitsPerPixel,
				SDL_HWSURFACE);
    }
    
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

void gui_EventHandler(void)
{
    bool quit_loop = false;
    SDL_Event event;

    while(quit_loop == false){
	SDL_WaitEvent(&event);
	//printf("SDL Event = %d\n",event.type);
	switch(event.type){
	case SDL_QUIT:
	    printf("SDL QUIT\n");
	    quit_loop = true;
	    break;
	case SDL_KEYDOWN:
	    printf("SDL_KEYDOWN key=%d\n",event.key.keysym.sym);
	    switch(event.key.keysym.sym){
	    case SDLK_0:
		quit_loop = true;
		break;
	    default:
		break;
	    }
	    break;
	default:
	    break;
	}
    }

}
