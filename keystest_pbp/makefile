TARGET = eboot
OBJS = eboot.o

INCDIR = ./inc ./inc_lib
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = ./lib ./inc_lib
LIBS = inc_lib/libpspdebug_ru.a inc_lib/libpspexploit.a -lpsprtc
LDFLAGS = -L.

PSP_FW_VERSION = 280
PSP_FW_VERSION_SFO = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Key[s]Tester
PSP_EBOOT_ICON = inc_lib/ICON0.PNG

ENCRYPT = 1

BUILD_PRX = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
