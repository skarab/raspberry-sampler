CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s
LDFLAGS=
LDLIBS=-L/usr/lib -lasound -lpthread

HEADERS=src/config.h src/bank.h src/device.h src/error.h src/includes.h src/log.h src/midi.h src/sample.h src/voice.h src/wav.h
SRCS=src/main.cpp src/config.cpp src/bank.cpp src/device.cpp src/error.cpp src/log.cpp src/midi.cpp src/sample.cpp src/voice.cpp src/wav.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: sampler

sampler: $(OBJS)
	$(CXX) $(LDFLAGS) -o sampler $(OBJS) $(LDLIBS)

%.o : %.cpp
	$(CXX) -c $< $(HEADERS) -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) sample



