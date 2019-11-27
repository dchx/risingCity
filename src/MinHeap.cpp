#include "risingcity.hpp"

// min heap node relations
int heapleft(int parent){return 2*parent + 1;}
int heapright(int parent){return 2*parent + 2;}
int heapparent(int child){return floor((child - 1)/2);}

void MinHeap::insert(Building * toinsert){
	// insert the building to the back of the array
	buildings.push_back(toinsert);
	if (buildings.size() > 2000)
		throw std::length_error("The number of active buildings has exceeded 2000.");
	// adjust position of the inserted building
	toinsert->heapind = buildings.size() - 1; // the index of the inserted buildings in buildings (last)
	int parentind;
	while (toinsert->heapind > 0){ // bottom up
		parentind = heapparent(toinsert->heapind);
		Building * parentBuilding = buildings.at(parentind);
		if (toinsert->executed_time < parentBuilding->executed_time){ // this building's executed_time smaller than parent
			std::swap(buildings.at(toinsert->heapind), buildings.at(parentind));
			std::swap(toinsert->heapind, parentBuilding->heapind);
		}
		else break;
	}
}
void topdown(std::vector<Building *> & buildings, Building * thisBuilding){
	/* top-down heapify from thisBuilding */
	int thisind, leftind, rightind;
	thisind = thisBuilding->heapind;
	while (1){ // top down
		leftind = heapleft(thisind);
		rightind = heapright(thisind);
		if (leftind < buildings.size()){ // this node is not leaf
			if (buildings.at(leftind)->executed_time < buildings.at(thisind)->executed_time)
				thisind = leftind;
			if (rightind < buildings.size()
			 && buildings.at(rightind)->executed_time < buildings.at(thisind)->executed_time)
				thisind = rightind;
			if (thisind != thisBuilding->heapind){
				std::swap(buildings.at(thisind), buildings.at(thisBuilding->heapind));
				std::swap(buildings.at(thisind)->heapind, buildings.at(thisBuilding->heapind)->heapind);
			}
			else break;
		}
		else break; // this node is leaf
	}
}
void MinHeap::remove(Building * toremove){
	// swap building toremove with the last element in the array
	Building * thisBuilding = buildings.at(buildings.size() - 1); // the last building
	std::swap(buildings.at(toremove->heapind), buildings.at(buildings.size() - 1));
	std::swap(buildings.at(toremove->heapind)->heapind, buildings.at(buildings.size() - 1)->heapind);
	// remove the swaped building toremove
	buildings.pop_back();
	// adjust position of the swaped thisBuilding
	topdown(buildings, thisBuilding);
}
void MinHeap::increasekey(Building * toincrease, int increaseval){
	// do increase key
	toincrease->executed_time += increaseval;
	// adjust position of the increased key
	topdown(buildings, toincrease);
}
