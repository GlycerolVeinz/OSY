CC = gcc
CFLAGS = -Wall
EXTRA_CFLAGS += 

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXEC = forkpipe

NSD_SRC = $(wildcard ./nsd/*.c)
NSD_OBJ = $(NSD_SRC:.c=.o)
NSD_EXEC = nsd.exe

ZIP_NAME = forkpipe.zip

all: $(EXEC) $(NSD_EXEC) 

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(NSD_EXEC): $(NSD_OBJ)
	$(CC) $(CFLAGS) -o $@ $(NSD_OBJ)



clean: clean_o clean_zip clean_exe

clean_o:
	rm -f *.o
	rm -f ./nsd/*.o

clean_zip:
	rm -f *.zip
	rm -f *.tar
	rm -f *.tgz
	
clean_exe:
	rm -f nsd.exe forkpipe

zip: clean
	zip -r $(ZIP_NAME) ./nsd ./Makefile ./*.c ./*.h