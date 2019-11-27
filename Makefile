OBJ = lib/MinHeap.o lib/RedBlackTree.o lib/City.o lib/risingcity.o lib/io.o
INCLUDE = -Iinclude

all: risingCity

risingCity: lib $(OBJ)
	g++ -o $@ $(OBJ)

lib/%.o: src/%.cpp include/risingcity.hpp
	g++ $(INCLUDE) -c $< -o $@

lib:
	mkdir lib

clean:
	rm -r lib
	rm risingCity
