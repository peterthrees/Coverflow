# Software Name
PROGRAM = coverflow.elf
PLATFORM = BITTBOY
#PLATFORM = PC

# Compiler


CC = g++


# You can use Ofast too but it can be more prone to bugs, careful.
#CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -no-pie
CFLAGS +=  -I.

LIBS += -lSDL_mixer -lSDL -lm -lSDL_image -lSDL_ttf -lSDL_gfx
LDFLAGS = -Wl,--start-group $(LIBS) -Wl,--end-group -Wl,--as-needed -Wl,--gc-sections -flto

ifeq ($(DEBUG), YES)
CFLAGS +=  -DDEBUG -g3
else
LDFLAGS	+=  -s
endif

CFILES =  main.cpp FileHandler.cpp CoverFlowOnScreen.cpp Configuration.cpp Helper.cpp CTime.cpp Screen.cpp
SFILES = 

OFILES = $(SFILES:.S=.o) $(CFILES:.cpp=.o)

$(PROGRAM):	$(OFILES)
			$(CC) $(CFLAGS) $(OFILES) -o $@ $(LDFLAGS)

all: $(PROGRAM)

%.o: %.c
	 $(CC) $(CFLAGS) -c $< -o $@

clean:
	 -rm -f $(OFILES) $(MAPFILE) $(PROGRAM)