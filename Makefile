SHELL = /bin/sh
CC = gcc
CFLAGS = -lcurl -Wall
TARGET = imgur
OBJECTS = imgur.c
PREFIX = $(DESTDIR)/usr
BINDIR = $(PREFIX)/bin
 
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	
install:
	install $(TARGET) $(BINDIR)

uninstall:
	-rm $(BINDIR)/$(TARGET)