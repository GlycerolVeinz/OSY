CC=gcc
CFLAGS=-Wall
EXTRA_CFLAGS+=

NSD_SRCS=$(wildcard ./nsd/*.c)
NSD_OBJS=$(NSD_SRCS:.c=.o)

nsd.exe: $(NSD_OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

%.o: nsd/%.c nsd/%.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<



clean: clean_o clean_zip clean_exe

clean_o:
	rm -f *.o

clean_zip:
	rm -f *.zip
	rm -f *.tar
	rm -f *.tgz
	
clean_exe:
	rm -f nsd/*.o nsd.exe