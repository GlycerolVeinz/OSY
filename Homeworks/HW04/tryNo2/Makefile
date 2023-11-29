CC = gcc
CFLAGS = -Wall -g -O2
EXTRA_CFLAGS := -fno-omit-frame-pointer -fsanitize=address
EXTRACFLAGS ?= $(EXTRA_CFLAGS)

TARGET=prod-cons
SOURCES=$(wildcard *.c)
DEPENDENCIES=$(wildcard *.h)

all: $(TARGET)

$(TARGET): $(SOURCES) $(DEPENDENCIES)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $(SOURCES)

clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f *.zip

zip: clean
	zip hw04.zip Makefile $(SOURCES) $(DEPENDENCIES)