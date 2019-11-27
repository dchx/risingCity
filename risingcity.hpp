#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

// buildings
struct Building{
	// items
	int buildingNum; // unique integer identifier for each building
	int executed_time; // total number of days spent so far on this building
	int total_time; // the total number of days needed to complete the construction of the building
	int heapind; // the index in the array as MinHeap.buildings
	// functions
	Building(int buildingNum, int total_time); // initialize
};

// min heap
int heapleft(int);
int heapright(int);
int heapparent(int);
struct MinHeap{
	// items
	std::vector<Building *> buildings; // the array to store data
	// functions
	void insert(Building *);
	void remove(Building *);
	void increasekey(Building *, int increaseval);
};

// red-black tree
enum colors {red, black};
struct RedBlackNode{
	// items
	enum colors color;
	RedBlackNode * parent;
	RedBlackNode * left;
	RedBlackNode * right;
	Building * building; // key is building->buildingNum
	// functions
	RedBlackNode * sibling();
	RedBlackNode(Building *);
};
struct RedBlackTree{
	/* used to store (buildingNum, executed_time, total_time) triplets ordered by buildingNum */
	// items
	RedBlackNode * root;
	// functions
	void insert(Building *);
	void remove(Building *);
	RedBlackNode * get(int buildingNum);
	std::vector<Building *> getrange(int buildingNum1, int buildingNum2);
	RedBlackTree();
};

// I/O operations
enum actions {Insert, Print};
struct IOterm{
	int time; // global time
	enum actions action; // Insert or Print
	std::vector<int> params; // {buildingNum, total_time} for Insert; {buildingNum1(, buildingNum2)} for Print
};
void readinput(std::string filename, std::vector<IOterm *> & actionlist);
void printbuilding(Building *, std::ofstream &);
void printbuildings(std::vector<Building *> &, std::ofstream &);
void printcomplete(Building *, int global_time, std::ofstream &);
// for testing
void printtree(RedBlackNode * root, int edge = 0); // print a red-black tree
void printheap(MinHeap * heap, int rootind = 0, int edge = 0); // print a min heap

// the city
struct City{
	// items
	std::vector<IOterm *> actionlist; // the list of input actions
	MinHeap * executedTimes;
	RedBlackTree * buildingNums;
	// functions
	void printbd(int buildingNum, std::ofstream &); // prints the triplet (buildingNum, executed_time, total_time)
	void printbds(int buildingNum1, int buildingNum2, std::ofstream &); // prints all triplets (buildingNum, executed_time, total_time) for buildingNum in [buildingNum1, buildingNum2]
	void insertbd(int buildingNum, int total_time); // where buildingNum is different from existing building numbers and executed_time = zero
	Building * selectbuilding(); // select the building with the lowest executed_time to work on
	void tackaction(int & global_time, int end_time, Building * workon, std::ofstream &); // perform actions on the actionlist with time in (start_time, end_time]
	void take_one_action(IOterm *, std::ofstream &); // perform one action of IOterm
	City();
};
