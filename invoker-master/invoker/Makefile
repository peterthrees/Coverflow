# Software Name
PROGRAM = invoker.elf
#PLATFORM = BITTBOY
PLATFORM = PC

# Compiler

ifeq ($(PLATFORM), BITTBOY)
CC = /opt/bittboy-toolchain/bin/arm-miyoo-linux-musleabi-g++
else ifeq ($(PROFILE), RS97)
CC = /opt/rs97-toolchain/bin/mipsel-linux-g++
else
CC = g++
endif

# You can use Ofast too but it can be more prone to bugs, careful.
ifeq ($(PLATFORM), BITTBOY)
CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie -static
else
CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie
endif

CFLAGS +=  -I.

LIBS += 
LDFLAGS = -Wl,--start-group $(LIBS) -Wl,--end-group -Wl,--as-needed -Wl,--gc-sections -flto

ifeq ($(DEBUG), YES)
CFLAGS +=  -DDEBUG -g3
else
LDFLAGS	+=  -s
endif

CFILES =  invoker.cpp
SFILES = 

OFILES = $(SFILES:.S=.o) $(CFILES:.cpp=.o)

$(PROGRAM):	$(OFILES)
			$(CC) $(CFLAGS) $(OFILES) -o $@ $(LDFLAGS)

all: $(PROGRAM)

%.o: %.cpp
	 $(CC) $(CFLAGS) -c $< -o $@

clean:
	 -rm -f $(OFILES) $(MAPFILE) $(PROGRAM)