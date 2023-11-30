CC = gcc
EXTRA_CFLAGS+= 
CFLAGS = -Wall $(EXTRA_CFLAGS)

SRC = $(wildcard *.c) 
OBJ = $(SRC:.c=.o)
HEADERS = $(wildcard *.h)
EXEC = forkpipe

NSD_SRC = $(wildcard ./dep/*.c)
NSD_OBJ = $(NSD_SRC:.c=.o)
NSD_HEADERS = $(wildcard ./dep/*.h)
NSD_EXEC = nsd

ZIP_NAME = forkpipe.zip

all: $(EXEC) $(NSD_EXEC) 

$(EXEC): $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(NSD_EXEC): $(NSD_OBJ) $(NSD_HEADERS)
	$(CC) $(CFLAGS) -o $@ $(NSD_OBJ)



clean: clean_o clean_zip clean_exe

clean_o:
	rm -f *.o
	rm -f *.d
	rm -f ./dep/*.o
	rm -f ./dep/*.d

clean_zip:
	rm -f *.zip
	rm -f *.tar
	rm -f *.tgz
	
clean_exe:
	rm -f nsd forkpipe




zip: clean
	zip -r $(ZIP_NAME) ./dep ./Makefile ./*.c ./*.h