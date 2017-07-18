CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s
LDFLAGS=
LDLIBS=-L/usr/lib -lasound -lpthread -lbcm2835

FILES=src/pugixml/src/pugixml.cpp \
	src/display.cpp \
	src/bank.cpp \
	src/sound.cpp \
	src/core.cpp \
	src/parameters_sample.cpp \
	src/parameters_global.cpp \
	src/helpers.cpp \
	src/error.cpp \
	src/log.cpp \
	src/midi.cpp \
	src/midi_key.cpp \
	src/sample.cpp \
	src/filters.cpp \
	src/voice.cpp \
	src/wav.cpp \
	src/button.cpp \
	src/led.cpp \
	src/knob_control.cpp \
	src/knob_select.cpp \
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
	$(RM) src/*.o

distclean: clean
	$(RM) sample



