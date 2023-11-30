CC = gcc
CFLAGS = -Wall -g -O2
EXTRA_CFLAGS := -fno-omit-frame-pointer -fsanitize=address
EXTRACFLAGS ?= $(EXTRA_CFLAGS)

TARGET=prod-cons
SOURCES=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SOURCES))
HEADERS=$(wildcard *.h)

all: $(TARGET)

$(TARGET): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(EXTRACFLAGS) -o $@ $^ -lpthread

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRACFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

zip : $(SOURCES) $(HEADERS)
	zip -r $(TARGET).zip $(SOURCES) Makefile $(HEADERS)
