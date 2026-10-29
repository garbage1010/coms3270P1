CC      = gcc
CFLAGS  = -Wall -g
LDLIBS  = -lm

# Objects shared by every executable
COMMON_OBJS = pointcloud.o util.o bmp.o
HEADERS     = pointcloud.h util.h bmp.h

all: watershed stata display

stata: pointstat.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

display: display.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
watershed: watershed.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Pattern rule: build any .o from the matching .c
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o watershed stata display out.gif

.PHONY: all clean