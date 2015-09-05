dict: dict.c bloom.o
	gcc -Wall -c dict.c 
	gcc dict.o bloom.o -lm -o dict

bloom.o: bloom.c bloom.h
	gcc -Wall -c bloom.c 

clean: 
	rm -f *~ bloom *.o
