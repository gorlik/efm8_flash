#efm8_flash makefile

SOURCES=*.c *.cpp
NAME=efm8


all: linux win

linux: $(SOURCES)
	mkdir -p linux
	( cd linux; $(MAKE) -f../Makefile.app APP=$(NAME))
	mv linux/$(NAME)_cli $(NAME)_cli
	mv linux/$(NAME)_flash $(NAME)_flash

win:
	mkdir -p win32
	( cd win32; $(MAKE) -f../Makefile.app APP=$(NAME) BUILD=WIN32)
	mv win32/$(NAME)_cli.exe $(NAME)_cli.exe
	mv win32/$(NAME)_flash.exe $(NAME)_flash.exe
	strip *.exe

all: linux win

clean:
	rm -rf *.a core.* \
	efm8_cli efm8_cli.exe efm8_flash efm8_flash.exe \
	linux win32 \
	*~
