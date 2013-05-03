#CROSS		 = arm-linux-gnueabihf-
CROSS		 =
CC		 = $(CROSS)gcc
CFLAGS		 = -I. -I./nmealib/include/ -L./nmealib/lib/ $(shell sdl-config --cflags --libs) -lSDL_image -lcurl -lpthread -lm -lnmea
CFLAGS		+= -Os
OBJS		 = main.o googlemap_api.o retrieve.o gps.o
TARGET		 = maptracer
UNZIP		 = unzip
NMEALIB		 = ./nmealib/lib/libnmea.a
NMEALIBDIR	 = ./nmealib
NMEALIBZIP	 = ./nmealib-0.5.3.zip

.PHONY: clean purge all

all: $(TARGET)

.c.o:
	@echo "========================================================"
	$(CC) $(CFLAGS) -c $<

$(NMEALIB):
	$(UNZIP) $(NMEALIBZIP)
	make -C $(NMEALIBDIR)

$(TARGET): $(NMEALIB) $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	@echo "Clean project =========================================="
	rm -fr $(TARGET) $(OBJS) ./nmealib

purge:
	rm -f *~ *bak

depend: $(OBJS:.o=.c)
	-@ makedepend -- $(CFLAGS) -- -Y $^ -f Makefile 2> /dev/null

