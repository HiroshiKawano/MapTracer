CROSS		 = 
CC		 = $(CROSS)gcc
CFLAGS		 = -O0 -I. `sdl-config --cflags --libs` -lSDL_image -lcurl -lpthread
OBJS		 = main.o googlemap_api.o retrieve.o
TARGET		 = maptracer

.PHONY: clean purge all

all:	$(TARGET)

.c.o:
	@echo "========================================================"
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	@echo "Clean project =========================================="
	rm -f *.bak $(TARGET) $(OBJS)

purge:
	rm -f *~ *bak

depend: $(OBJS:.o=.c)
	-@ makedepend -- $(CFLAGS) -- -Y $^ -f Makefile 2> /dev/null



