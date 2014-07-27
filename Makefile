SHELL = /bin/sh
CC = gcc
CFLAGS = -lcurl
TARGET = imgur
OBJECTS = imgur.c
PREFIX = $(DESTDIR)/usr
BINDIR = $(PREFIX)/bin
 
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	
install:
	install imgur $(BINDIR)

uninstall:
	-rm $(BINDIR)/$(TARGET)