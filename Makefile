CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O3 -s -std=c99
LDFLAGS=
LDLIBS=-L/usr/lib -lasound

SRCS=main.c
OBJS=$(subst .c,.o,$(SRCS))

all: synth

synth: $(OBJS)
	$(CXX) $(LDFLAGS) -o synth $(OBJS) $(LDLIBS)

main.o: main.c

clean:
	$(R%) $(OBJS)

distclean: clean
	$(RM) synth



