#include "risingcity.hpp"

// ---------- Building ---------- //
Building::Building(int num, int time){
	buildingNum = num;
	executed_time = 0;
	total_time = time;
	heapind = -1;
}

// ---------- City ---------- //
City::City(){
	/* initialization */
	actionlist.clear();
	buildingNums = new RedBlackTree();
	executedTimes = new MinHeap;
}
void City::insertbd(int buildingNum, int total_time){
	/* insert a building with buildingNum and total_time to the city */
	Building * toinsert = new Building(buildingNum, total_time);
	buildingNums->insert(toinsert);
	executedTimes->insert(toinsert);
}
void City::printbd(int buildingNum, std::ofstream & outfile){
	/* print a building with buildingNum to outfile */
	RedBlackNode * toprint = buildingNums->get(buildingNum);
	printbuilding(toprint->building, outfile);
	outfile << std::endl;
}
void City::printbds(int buildingNum1, int buildingNum2, std::ofstream & outfile){
	/* print buildings within [buildingNum1, buildingNum2] to outfile */
	std::vector<Building *> toprint = buildingNums->getrange(buildingNum1, buildingNum2);
	printbuildings(toprint, outfile);
}
std::vector<int> breaktie(std::vector<Building *> & buildings, int root, int min_time){
	/* return {index, buildingNum} of the selected building 
	 * assuming root is in the heap and root has min_time */
	std::vector<int> leftmin = {};
	std::vector<int> rightmin = {};
	std::vector<int> globalmin = {root, buildings.at(root)->buildingNum};
	int leftind = heapleft(root);
	int rightind = heapright(root);
	if (leftind < buildings.size())
		if (buildings.at(leftind)->executed_time == min_time){ // left is tie
			leftmin = breaktie(buildings, leftind, min_time);
			if (leftmin.at(1) < globalmin.at(1)) // pick smaller buildingNum
				globalmin = leftmin;
		}
	if (rightind < buildings.size())
		if (buildings.at(rightind)->executed_time == min_time){ // right is tie
			rightmin = breaktie(buildings, rightind, min_time);
			if (rightmin.at(1) < globalmin.at(1)) // pick smaller buildingNum
				globalmin = rightmin;
		}
	return globalmin;
}
Building * City::selectbuilding(){
	/* select the building with the lowest executed_time to work on */
	if (executedTimes->buildings.size() == 0) return nullptr;
	if (executedTimes->buildings.size() == 1) return executedTimes->buildings.at(0);
	// break executed_time ties by selecting the building with the lowest buildingNum
	std::vector<int> selected; // {heapindex, buildingNum} of selected building
	selected = breaktie(executedTimes->buildings, 0, executedTimes->buildings.at(0)->executed_time);
	return executedTimes->buildings.at(selected.at(0));
}
void City::take_one_action(IOterm * term, std::ofstream & outfile){
	/* take one Print/Insert action as instructed in the IOterm */
	if (term->action == Insert){
		insertbd(term->params.at(0), term->params.at(1));
	}
	if (term->action == Print){
		if (term->params.size() == 1)
			printbd(term->params.at(0), outfile);
		if (term->params.size() == 2)
			printbds(term->params.at(0), term->params.at(1), outfile);
	}
}
void City::tackaction(int & global_time, int end_time, Building * workon, std::ofstream & outfile){
	/* tack actions on the actionlist with time in (start_time, end_time]
	 * workon: the building being worked on
	 * assuming time of actions on actionlist is sorted */
	while (actionlist.size() != 0 && actionlist.front()->time <= end_time){
		if (workon != nullptr) executedTimes->increasekey(workon, actionlist.front()->time - global_time); // update workon executed time
		take_one_action(actionlist.front(), outfile);
		global_time = actionlist.front()->time;
		actionlist.erase(actionlist.begin());
	}
	if (workon != nullptr) executedTimes->increasekey(workon, end_time - global_time); // update executed time of workon
	global_time = end_time;
}
