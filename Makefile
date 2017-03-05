CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s -std=c99
LDFLAGS=
LDLIBS=-L/usr/lib -lasound -lpthread -lncurses

SRCS=main.cpp device.cpp error.cpp sample.cpp voice.cpp wav.cpp
OBJS=$(subst .c,.o,$(SRCS))

all: sampler

sampler: $(OBJS)
	$(CXX) $(LDFLAGS) -o sampler $(OBJS) $(LDLIBS)

main.o: main.cpp

clean:
	$(R%) $(OBJS)

distclean: clean
	$(RM) sample



