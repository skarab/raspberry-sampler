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
	src/midi_key.cpp \
	src/sample.cpp \
	src/voice.cpp \
	src/filter.cpp \
	src/filter_stereo.cpp \
	src/filter_overdrive.cpp \
	src/filter_noise.cpp \
	src/filter_bitcrusher.cpp \
	src/filter_distortion.cpp \
	src/filter_highpass.cpp \
	src/filter_lowpass.cpp \
	src/filter_eq.cpp \
	src/filter_formant.cpp \
	src/filter_moog.cpp \
	src/filter_notch.cpp \
	src/filter_test.cpp \
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
	$(RM) $(OBJS)

distclean: clean
	$(RM) sample



