ifeq ($(prefix),)
	prefix=/usr/local
endif
ifeq ($(datadir),)
	datadir=$(prefix)/share
endif
ifeq ($(shell which $(CC)),)
	CC=gcc
endif
all:
	$(CC) $(CFLAGS) -o cave4 cave4.c -lcurses
	strip -S cave4
install:
	if [ ! -d "$(DESTDIR)$(prefix)/games" ]; then mkdir -p $(DESTDIR)$(prefix)/games; fi
	install -m755 cave4 $(DESTDIR)$(prefix)/games
clean:
	rm cave4
