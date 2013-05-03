/*
  Simple map data retriever for Google Static Maps API V2

  Version 5/1/2013

  Coded by Yasuhiro ISHII
  This code is distributed under Apache2.0 license
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <curl/curl.h>

#define HTTP_HEADER_CONTENT_LENGTH	"Content-Length: "

#define CONTENT_MAX_SIZE		(1024*1024)

//#define DEBUG

int current_content_size = -1;
void* content_buffer = NULL;

static size_t retriever_write_callback(char* ptr,size_t size,size_t nmemb,void* userdata);
static size_t retriever_header_callback(char* ptr,size_t size,size_t nmemb,void* userdata);

static size_t retriever_write_callback(char* ptr,size_t size,size_t nmemb,void* userdata)
{
    int ret = nmemb*size;
    static int current_pos = 0;
    
    if(content_buffer == NULL){
	if(current_content_size != -1 && current_content_size < CONTENT_MAX_SIZE){
	    content_buffer = malloc(current_content_size);
	    current_pos = 0;
	    if(content_buffer == NULL){
		printf("%s : memory allocation error\n",__func__);
		ret = 0;
	    }
	}
    }
    
    if(content_buffer != NULL){
	memcpy(content_buffer+current_pos,ptr,nmemb*size);
	current_pos += size*nmemb;
#ifdef DEBUG
	printf("curpos = %d\n",current_pos);
#endif
    }
    
    return(ret);
}

static size_t retriever_header_callback(char* ptr,size_t size,size_t nmemb,void* userdata)
{
    // Memo:
    // parse header with alternatively allocated "buff" to handle it as like MS's _snscanf()
    
    char* buff = malloc(size*nmemb+1);
    int len = strlen(HTTP_HEADER_CONTENT_LENGTH);
    
    if(buff == NULL){
	printf("%s : Memory allocation error\n",__func__);
	return(-1);
    }
    memcpy((void*)buff,(const void*)ptr,size*nmemb);
    buff[size*nmemb] = 0; // add terminator
    
    if(strncmp(HTTP_HEADER_CONTENT_LENGTH,buff,len) == 0){
	sscanf(buff+len,"%d",&current_content_size);
	printf("CONTENT-SIZE = %ld\n",current_content_size);
    }
    
    free(buff);
    
    return(nmemb*size);
}


bool http_retrieve(unsigned char* url,bool (*callback)(void* buff,int len))
{
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
	current_content_size = -1;
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &retriever_write_callback);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &retriever_header_callback);
	
	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl);

	/* Check for errors */
	if(res != CURLE_OK){
	    fprintf(stderr, "curl_easy_perform() failed: %s\n",
		    curl_easy_strerror(res));
	} else {

	  (*callback)(content_buffer,current_content_size);
#if 0
	    {
		FILE* f;
		f = fopen("for_debug.png","wb");
		fwrite(content_buffer,current_content_size,1,f);
		fclose(f);
	    }
#endif
	    free(content_buffer);
	    content_buffer = NULL;
	}
	/* always cleanup */
	curl_easy_cleanup(curl);
    }
    return true;
}

