OBJ = MinHeap.o RedBlackTree.o City.o risingcity.o io.o

all: risingCity

risingCity: $(OBJ)
	g++ -o $@ $(OBJ)

%.o: %.cpp risingcity.hpp
	g++ -c $< -o $@

lib:
	mkdir lib

clean:
	rm -f *.o risingCity
