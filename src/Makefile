#CROSS		 = arm-linux-gnueabihf-
CROSS		 =
CC		 = $(CROSS)gcc
CFLAGS		 = -O0 -I. `sdl-config --cflags --libs` -lSDL_image -lcurl -lpthread
OBJS		 = main.o googlemap_api.o retrieve.o
TARGET		 = maptracer
UNZIP		 = unzip
NMEALIB		 = ./nmealib/lib/libnmea.a
NMEALIBDIR	 = ./nmealib

.PHONY: clean purge all

all:	$(TARGET)

.c.o:
	@echo "========================================================"
	$(CC) $(CFLAGS) -c $<

$(NMEALIB):
	$(UNZIP) ./nmealib-0.5.3.zip
	make -C $(NMEALIBDIR)

$(TARGET): $(OBJS) $(NMEALIB)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	@echo "Clean project =========================================="
	rm -fr *.bak $(TARGET) $(OBJS) ./nmealib

purge:
	rm -f *~ *bak

depend: $(OBJS:.o=.c)
	-@ makedepend -- $(CFLAGS) -- -Y $^ -f Makefile 2> /dev/null



