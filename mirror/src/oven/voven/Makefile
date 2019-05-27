CC              = cc68k
CFLAGSA         = -DCPU=MC68030 -I$(VX_VW_BASE)/h
CFLAGSAL        = $(CFLAGSA) -traditional
CFLAGSALL       = $(CFLAGSAL) -DVXWORKS
COMPILE.c       = $(CC) $(CFLAGS) $(CPPFLAGS) -c

OBJS1           = oven.o cwork.o pwork.o twork.o util.o tmath.o isr.o \
		  getiptime.o portrw.o tip.o
OBJS2		= adc.o dculib.o sio.o

OBJS		= $(OBJS1) $(OBJS2)

$(OBJS1)        := CFLAGS = $(CFLAGSALL) -O
$(OBJS2)        := CFLAGS = $(CFLAGSALL) -fvolatile

voven.o: $(OBJS)
	ld68k -o voven.o -r $(OBJS)

oven.o:		oven.h
cwork.o:	oven.h
pwork.o:	oven.h
twork.o:	oven.h
util.o:		oven.h
isr.o:		oven.h
dculib.o:	opio.h

clean:
	rm -f $(OBJS)
