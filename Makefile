OBJ = MinHeap.o RedBlackTree.o io.o City.o risingcity.o

all: risingCity

risingCity: $(OBJ)
	g++ -o $@ $(OBJ)

%.o: %.cpp risingcity.hpp
	g++ -c $< -o $@

clean:
	rm -f *.o risingCity
