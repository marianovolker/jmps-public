SYSROOT = /opt/devkit-x86/sysroots/i586-poky-linux
CC = i586-poky-linux-g++
CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0 --sysroot=$(SYSROOT) -m32 -march=i586 -c -ffunction-sections -fdata-sections
INCLUDES = -I/opt/devkit-x86/sysroots/i586-poky-linux/usr/include/upm
LFLAGS = --sysroot=$(SYSROOT)
LIBS = -lmraa
SRCS = write_relay_handler.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = write_relay_handler

.PHONY: clean

all: $(MAIN)
	@echo  Simple compiler named write_relay_handler has been compiled

$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
