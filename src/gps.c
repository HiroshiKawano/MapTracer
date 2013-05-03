#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <math.h>

#include <nmea/nmea.h>

//#define DEBUG

double convert_radian_to_degree(double rad);

void trace(const char *str, int str_size)
{
#ifdef DEBUG
    printf("Trace: ");
    write(1, str, str_size);
    printf("\n");
#endif
}

void error(const char *str, int str_size)
{
    printf("Error: ");
    write(1, str, str_size);
    printf("\n");
}

int set_interface_attribs (int fd, int speed)
{
    struct termios tty;

    memset (&tty, 0, sizeof tty);

    // retrieve current settings
    if (tcgetattr (fd, &tty) != 0){
	printf ("error %d from tcgetattr", errno);
	return -1;
    }

    if(cfsetspeed (&tty, B4800) != 0){
	printf("cfsetospeed returns error\n");
	return(-1);
    }

    tty.c_lflag = ICANON;            // use canonical mode
    
    tty.c_cflag = CS8;
    tty.c_cc[VMIN]  = 10;              // read doesn't block
    tty.c_cc[VTIME] = 10;             // 1 second(s) read timeout
    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_oflag = 0;

    if (tcsetattr (fd, TCSANOW, &tty) != 0){
	printf ("error %d from tcsetattr", errno);
	return -1;
    }
    return 0;
}

#ifdef DEBUG
int flag = 0;
#endif

bool gpsRetriever(char* interface)
{
    int fd;
    int result;
    unsigned char buff[4096];
    nmeaINFO info;
    nmeaPARSER parser;
    nmeaPOS dpos;
    int it = 0;
    
    fd = open(interface,O_RDWR | O_NOCTTY | /*O_NONBLOCK */O_NDELAY );
    
    set_interface_attribs(fd,B4800);
    
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &trace;
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
    
    while(1){
	
	result = read(fd,buff,sizeof(buff));
	
	if(result > 0){
	    buff[result] = 0;
	    
#ifdef DEBUG
	    if(flag == 0){
		printf("\x1b[31m");
		flag = 1;
	    } else {
		printf("\x1b[33m");
		flag = 0;
	    }
	    printf("%s",buff);
#endif /* DEBUG */
	    
	    nmea_parse(&parser, &buff[0], result, &info);
	    nmea_info2pos(&info, &dpos);
	    
	    printf(
		"%03d, Pos( %lf,%lf ), Sig: %d, Fix: %d, Elv : %lf Dir :%lf,Sat inview : %d,Sat inuse : %d\n",
		it++,
		convert_radian_to_degree(dpos.lat),
		convert_radian_to_degree(dpos.lon),
		info.sig,
		info.fix,
		info.elv,
		info.direction,
		info.satinfo.inview,
		info.satinfo.inuse
		);
	}
	
    }
    
    nmea_parser_destroy(&parser);
    close(fd);
}

double convert_radian_to_degree(double rad)
{
    return (rad / (M_PI/180));
}
