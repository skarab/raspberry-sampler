CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s
LDFLAGS=
LDLIBS=-L/usr/lib -lasound -lpthread -lncurses

SRCS=src/main.cpp src/device.cpp src/error.cpp src/sample.cpp src/voice.cpp src/wav.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: sampler

sampler: $(OBJS)
	$(CXX) $(LDFLAGS) -o sampler $(OBJS) $(LDLIBS)

%.o : %.cpp
	$(CXX) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) sample



