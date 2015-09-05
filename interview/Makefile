all: libraryserv libraryclient

libraryclient: libraryclient.c common.o
	gcc  -Wall -o libraryclient libraryclient.c common.o

libraryserv: libraryserv.c common.o
	gcc  -Wall -o libraryserv libraryserv.c common.o

common.o: common.h common.c
	gcc  -Wall -c common.c

serv: libraryserv
	./libraryserv -p 5001 -a localhost

client: libraryclient
	./libraryclient -p 5001 -a localhost

clean:
	rm -f libraryclient libraryserv *.o
	
