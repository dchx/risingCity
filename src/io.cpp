#include "risingcity.hpp"

void getaction(std::string & line, IOterm * thisterm){
	/* convert a line of input command into an IOterm */
	//   action time
	thisterm->time = std::stoi(line.substr(0, line.find_first_of(":")));
	//   action name
	if (line.find("Insert")!=std::string::npos) thisterm->action = Insert;
	if (line.find("Print")!=std::string::npos) thisterm->action = Print;
	//   action parameters
	int leftind = line.find_first_of("(") + 1;
	int rightind = line.find_last_of(")");
	std::string parastr(line, leftind, rightind - leftind); // substring with parameters
	int start = 0; int end;
	for (size_t i = 0; i < parastr.size(); i++){
		if (parastr.at(i) == ','){
			end = i;
			thisterm->params.push_back(std::stoi(parastr.substr(start,end)));
			start = i+1;
		}
		if (i == parastr.size()-1)
			thisterm->params.push_back(std::stoi(parastr.substr(start,i+1)));
	}
}
void readinput(std::string filename, std::vector<IOterm *> & actionlist){
	/* read actions from input filename and convert them to actionlist */
	std::string line;
	std::ifstream infile(filename);
	if (infile.is_open()){
		while (std::getline(infile,line)){
			IOterm * thisterm = new IOterm;
			getaction(line, thisterm);
			actionlist.push_back(thisterm);
		}
		infile.close();
	}
}
void printbuilding(Building * building, std::ofstream & outfile){
	if (outfile.is_open()){
		outfile << "(" << building->buildingNum << ","
		                 << building->executed_time << ","
		                 << building->total_time << ")";
	}
}
void printbuildings(std::vector<Building *> & buildings, std::ofstream & outfile){
	if (outfile.is_open()){
		for (size_t i = 0; i < buildings.size(); i++){
			printbuilding(buildings.at(i), outfile);
			if (i < buildings.size() - 1) outfile << ",";
		}
		outfile << std::endl;
	}
}
void printcomplete(Building * building, int global_time, std::ofstream & outfile){
	if (outfile.is_open()){
		outfile << "(" << building->buildingNum << ","
		                 << global_time << ")" << std::endl;
	}
}
void printtree(RedBlackNode * root, int edge){
	int tab = 10;
	if (root == nullptr) return;
	edge += tab;
	printtree(root->right, edge);
	std::cout << std::endl;
	for (size_t i = tab; i < edge; i++) std::cout<<" ";
	std::cout << root->building->buildingNum << "(" << (root->color == red? "r":"b") << ")" << std::endl;
	printtree(root->left, edge);
}
void printheap(MinHeap * heap, int rootind, int edge){
	int tab = 10;
	if (rootind < 0 || rootind >= heap->buildings.size()) return;
	edge += tab;
	printheap(heap, heapright(rootind), edge);
	std::cout << std::endl;
	for (size_t i = tab; i < edge; i++) std::cout<<" ";
	std::cout << heap->buildings.at(rootind)->executed_time << "(" << heap->buildings.at(rootind)->buildingNum << ")" << std::endl;
	printheap(heap, heapleft(rootind), edge);
}
