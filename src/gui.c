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
#include <time.h>
#include "gui.h"
#include "gps.h"
#include "handle_googleapi_key.h"

//#define DEBUG

bool gui_open(bool cvbs,bool no_gui);
bool gui_close(void);
bool showImageFileOnMemory_FullScreen(void* data,int size,int format);
bool showPngFile_FullScreen(unsigned char *filename);

static bool gui_DrawMaps(int mode);
static void http_draw_callback(void* buff,int len);

static bool cvbs_resolution = false;
static bool no_gui_mode = false;
static bool demo_mode = false;

enum gui_DrawMode {
    GUI_DRAWMODE_GOOGLEMAPS,
    GUI_DRAWMODE_GOOGLESTREETVIEW
};

static enum gui_DrawMode draw_mode = GUI_DRAWMODE_GOOGLEMAPS;

bool gui_open(bool cvbs,bool no_gui)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
	printf("SDL_Init failed (%s)\n",SDL_GetError());
	return false;
    }
    cvbs_resolution = cvbs;
    no_gui_mode = no_gui;

    return true;
}

bool gui_close(void)
{
    SDL_Quit();
    return true;
}

bool showImageFileOnMemory_FullScreen(void* data,int size,int format)
{
    SDL_Surface* screen = NULL;
    SDL_Surface* image = NULL;
    SDL_RWops *rw;
    const SDL_VideoInfo *videoInfo = NULL;
    
    if(no_gui_mode == true){
	return(true);
    }

    if((format != ImageFormat_JPEG)&&(format != ImageFormat_PNG)){
	// format specifying error
	return(false);
    }

    videoInfo = SDL_GetVideoInfo();
    if(videoInfo == 0){
	printf("SDL_GetVideoInfo failed (%s)\n",SDL_GetError());
	return(false);
    }
    
    rw = SDL_RWFromMem(data,size);

    if(format == ImageFormat_PNG){
	image = (SDL_Surface*)IMG_LoadPNG_RW(rw,0);
    } else if (format == ImageFormat_JPEG){
	image = (SDL_Surface*)IMG_LoadJPG_RW(rw,0);
    }	

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

    if(no_gui_mode == true){
	return(true);
    }
    
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
    int result;

    //clock_gettime(CLOCK_MONITONIC,&tp);

    while(quit_loop == false){
	result = SDL_PollEvent(&event);
	if(result == 1){
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
		case SDLK_1:
		    printf("set mode to GoogleMaps\n");
		    draw_mode = GUI_DRAWMODE_GOOGLEMAPS;
		    break;
		case SDLK_2:
		    printf("set mode to GoogleStreetView\n");
		    draw_mode = GUI_DRAWMODE_GOOGLESTREETVIEW;
		    break;
		default:
		    break;
		}
		break;
	    default:
		break;
	    }
	} else {
	    
	    /* TODO;
	       modify this code to check with clock_gettime(CLOCK_MONITONIC,)
	    */
	    usleep(500000);
	    
	    gui_DrawMaps(draw_mode);
	}
    }

}

static bool gui_DrawMaps(int mode)
{
    struct gpsParams gps_param;
    bool result;
    unsigned char url[100];

    gpsGetCurrentParams(&gps_param);

    if(mode == 0){
	result = googlemaps_createUrl_FromLatLon(gps_param.lat,gps_param.lon,GOOGLE_API_KEY,url);
    } else {
	result = googlestreetview_createUrl_FromLatLon(gps_param.lat,gps_param.lon,(int)gps_param.dir,GOOGLE_API_KEY,url);
    }

    if(result == true){
	http_retrieve(url,http_draw_callback);
    } else {
	printf("createUrl_FromLatLon failed\n");
    }
}

void http_draw_callback(void* buff,int len)
{
    // !!!!! temporarily implementation (about mode)
    // TODO;Need to fix later!

    if(draw_mode == GUI_DRAWMODE_GOOGLEMAPS){
	showImageFileOnMemory_FullScreen(buff,len,ImageFormat_PNG);
    } else {
	showImageFileOnMemory_FullScreen(buff,len,ImageFormat_JPEG);
    }
}

