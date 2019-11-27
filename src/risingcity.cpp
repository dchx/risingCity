#include "risingcity.hpp"

int main(int argc, char* argv[]){
	int global_time = 0;
	int max_work_time = 5;
	int remaining_time, work_time;
	std::ofstream outfile("output_file.txt");
	City * risingCity = new City();
	readinput(argv[1], risingCity->actionlist);
	while (1){
		Building * workon = risingCity->selectbuilding();
		if (workon == nullptr){ // nothing to be work on
			if (risingCity->actionlist.size() == 0) return 0;
			risingCity->tackaction(global_time, risingCity->actionlist.at(0)->time, workon, outfile); // perform next I/O action and update global_time
		}
		else { // has selected something to work on
			remaining_time = workon->total_time - workon->executed_time;
			work_time = (remaining_time <= max_work_time)? remaining_time : max_work_time;
			risingCity->tackaction(global_time, global_time + work_time, workon, outfile); // perform I/O actions and update global_time
			if (work_time == remaining_time){ // building finish
				printcomplete(workon, global_time, outfile);
				risingCity->executedTimes->remove(workon);
				risingCity->buildingNums->remove(workon);
			}
		}
	}
	outfile.close();
	return 0;
}
