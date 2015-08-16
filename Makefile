CC		:=	gcc
ODIR	:=	build
SDIR	:=	source
IDIR	:=	-Iinclude
LDIR	:=	lib
CFLAGS	:=	$(IDIR) -fms-extensions -std=c11 -O2 -Wall
LFLAGS	:=	-L$(LDIR) -lws2_32
CFILES	:=	$(wildcard $(SDIR)/*.c)
OBJS	:=	$(patsubst $(SDIR)/%.c, build/%.o, $(wildcard $(SDIR)/*.c))

PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)), Linux)
	TARGET = $(shell basename $(CURDIR))
endif

ifeq ($(findstring Darwin,$(PLATFORM)), Darwin)
	TARGET = $(shell basename $(CURDIR))
endif

ifeq ($(findstring MINGW,$(PLATFORM)), MINGW)
	TARGET = $(shell basename $(CURDIR)).exe
endif

$(TARGET): $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $< -c -o $@ $(CFLAGS) 

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o
