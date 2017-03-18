CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s -DENABLE_HARDWARE=1
LDFLAGS=
LDLIBS=-L/usr/lib -lasound -lpthread -lbcm2835

FILES=src/pugixml/src/pugixml.cpp \
	src/display.cpp \
	src/bank.cpp \
	src/device.cpp \
	src/helpers.cpp \
	src/error.cpp \
	src/log.cpp \
	src/midi.cpp \
	src/sample.cpp \
	src/voice.cpp \
	src/wav.cpp \
	src/button.cpp \
	src/knob.cpp \
	src/controller.cpp

SRCS=src/main.cpp  $(FILES)
#HEADERS=src/config.h src/includes.h $(subst .cpp,.h,$(FILES))
HEADERS=
OBJS=$(subst .cpp,.o,$(SRCS))

all: sampler

sampler: $(OBJS)
	$(CXX) $(LDFLAGS) -o ../sampler $(OBJS) $(LDLIBS)

%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< $(HEADERS) -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) sample



