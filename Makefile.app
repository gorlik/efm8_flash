VPATH=..
INCLUDE=-I..

CXXFLAGS=-Wall -DUSE_WX $(INCLUDE) -ggdb
CFLAGS=$(INCLUDE)

OBJECTS_CLI = efm8_cli.o

OBJECTS_FLASH = efm8_flash_gui.o efm8_flash_lib.o

OBJECTS=$(OBJECTS_CLI) $(OBJECTS_FLASH)

ifeq ($(BUILD),WIN32)
#windows configuration

AR=i686-w64-mingw32-ar
CC=i686-w64-mingw32-gcc
CXX=i686-w64-mingw32-g++

WXCONFIG=/usr/local/i686-w64-mingw32/bin/wx-config
BF=-win32

CFLAGS+=-I/usr/local/i686-w64-mingw32/include
CXXFLAGS+=-I/usr/local/i686-w64-mingw32/include
LIBS+=-static -L/usr/local/i686-w64-mingw32/lib -lhidapi -lsetupapi -lole32 -ladvapi32

OBJECTS_GUI+=gg_icon.o

else
# linux configuration

WXCONFIG=wx-config
LIBS+=-lhidapi-libusb -lpthread

endif


ifeq ($(DEBUG),YES)
        CXXFLAGS+= `$(WXCONFIG) --cxxflags --debug=yes` -ggdb
	WXLIBS= `$(WXCONFIG) --libs --debug=yes`
else
        CXXFLAGS+= `$(WXCONFIG) --cxxflags`
	WXLIBS= `$(WXCONFIG) --libs`
endif

$(APP): $(OBJECTS)
	$(CC) -o $(APP)_cli $(OBJECTS_CLI) $(LIBS)
	$(CXX) -o $(APP)_flash $(OBJECTS_FLASH) $(LIBS) $(WXLIBS)


efm8_cli.o: efm8_flash_lib.c
	$(CC) $(CFLAGS) -D CLI_APP -c -o $@ $<

efm8_flash_lib.o: efm8_flash_lib.c
	$(CXX) $(CFLAGS) -c -o $@ $<

gg_icon.o: efm8_flash.rc gg.ico
	i686-w64-mingw32-windres $< gg_icon.o

clean:
	rm -f *.o $(NAME) core.*
