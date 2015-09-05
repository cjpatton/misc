flags=-Wall -g 

ziv: ziv.o huffman.o minheap.o lempel.o
	$(CXX) $(flags) -o ziv ziv.o huffman.o minheap.o lempel.o

ziv.o: ziv.cpp huffman.h 
	$(CXX) $(flags) -c ziv.cpp

huffman.o: huffman.cpp huffman.h minheap.h 
	$(CXX) $(flags) -c huffman.cpp

minheap.o: minheap.cpp minheap.h
	$(CXX) $(flags) -c minheap.cpp

hashtable.o: hashtable.cpp hashtable.h
	$(CXX) $(flags) -c hashtable.cpp

lempel.o: lempel.cpp lempel.h hashtable.h
	$(CXX) $(flags) -c lempel.cpp

dist: clean
	tar -zcf ziv.tgz *.h *.cpp Makefile

clean: 
	rm -f *~ *.o ziv test

test.o: test.cpp lempel.h
	$(CXX) $(flags) -c test.cpp

test: test.o lempel.o hashtable.o
	$(CXX) $(flags) -o test test.o lempel.o hashtable.o

run: ziv
	#echo -n 'ABBAACCDDABBBCCAAAAAAAA' > file
	./ziv < dij > zipped
	./ziv -d < zipped > unzipped
	diff unzipped dij


