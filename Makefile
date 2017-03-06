CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s -D ENABLE_GPIO=1
LDFLAGS=
LDLIBS=-L/usr/lib -lasound -lpthread -lbcm2835

FILES=src/controller.cpp src/preset.cpp src/device.cpp src/error.cpp src/log.cpp src/midi.cpp src/sample.cpp src/voice.cpp src/wav.cpp src/tinyxml/tinyxml2.cpp
SRCS=src/main.cpp  $(FILES)
HEADERS=src/config.h src/includes.h $(subst .cpp,.h,$(FILES))
OBJS=$(subst .cpp,.o,$(SRCS))

all: sampler

sampler: $(OBJS)
	$(CXX) $(LDFLAGS) -o bin/sampler $(OBJS) $(LDLIBS)

%.o : %.cpp
	$(CXX) -c $< $(HEADERS) -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) sample



