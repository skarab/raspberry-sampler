CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s -D ENABLE_GPIO=1
LDFLAGS=
LDLIBS=-L/usr/lib -lasound -lpthread -lbcm2835

FILES=controller.cpp preset.cpp device.cpp error.cpp log.cpp midi.cpp sample.cpp voice.cpp wav.cpp
SRCS=main.cpp  $(FILES)
HEADERS=config.h includes.h $(subst .cpp,.h,$(FILES))
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



