run: BTree
	./BTree

BTree.o: BTree.cpp
	g++ -c BTree.cpp

BTree: BTree.o
	g++ BTree.o -o BTree

clean:
	rm -f *.o
	rm BTree
