#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

string times[48] = { "12a", "12:30a", "1:00a", "1:30a", "2:00a", "2:30a", "3:00a", "3:30a", "4:00a", "4:30a", "5:00a", "5:30a", "6:00a", "6:30a", "7:00a", "7:30a", "8:00a", "8:30a", "9:00a", "9:30a",
						"10:00a", "10:30a", "11:00a", "11:30a", "12:00p", "12:30p", "1:00p", "1:30p", "2:00p", "2:30p", "3:00p", "3:30p", "4:00p", "4:30p", "5:00p", "5:30p", "6:00p", "6:30p", "7:00p", "7:30p",
						"8:00p", "8:30p", "9:00p", "9:30p", "10:00p","10:30p", "11:00p", "11:30p" };
int starttime;
int endtime;
string chart[100][100];
int LGcount_pershift[50];
ifstream myfile;
ofstream outputfile;
int average;
int loopcounter = 0;

struct Lifeguard {
	bool down = true;
	int standcounter = 0;
	string name = "No_name";
	int starttime = 0;
	int endtime = 0;
	bool disperse_viable = true;
};

struct Stands {
	string name;
	int startime = 0;
	int endtime = 0;
};

int SearchChart(int column, string stand, int count) { // searches the given column of the chart for the given name of the stand and returns the y value of it

	for (int y = 0; y < count; y++) {
		if (chart[column][y] == stand) {
			return y;
		}
	}


	return -1;
}

void StandInput(Stands A[], int count){ 

	for (int i = 0; i < count; i++) {
		//cout << "Enter the name of the stand:";
		myfile >> A[i].name;
	}

	return;
}

int findtime(string input) { //return index of time input


	for (int i = 0; i < 48; i++) {
		if (input == times[i])
			return i;
	}
	return -1;
}


int shiftInput(int start[], int end[], Lifeguard L[]) {
	int shifts = 0;
	int LGcount_input = 0;
	int LGcount = 0;
	string timeinput;
	cout << "Enter the amount of shifts you are going to Input:";
	cin >> shifts;

	if (shifts > 0) {
		for (int i = 0; i < shifts; i++) {
			while (true) {
				cout << "Shift start time:";
				cin >> timeinput;
				start[i] = findtime(timeinput) - starttime;
				if (start[i] == -1) {
					cout << "Please enter a valid start time" << endl;
				}
				else
					break;
			}
			while (true) {
				cout << "Shift end time";
				cin >> timeinput;
				end[i] = findtime(timeinput) - starttime;
				if (end[i] == -1)
					cout << "Please enter a valid end time" << endl;
				else
					break;

			}
			cout << "Amount of lifeguards per shift:";
			cin >> LGcount_input;

			LGcount += LGcount_input;
			LGcount_pershift[i] = LGcount_input;


			for (int j = LGcount - LGcount_input; j < LGcount; j++) {
				L[j].starttime = start[i];
				L[j].endtime = end[i];
				if (L[j].endtime - L[j].starttime <= 12)
					L[j].disperse_viable = false;
			}
		}

	}


	return LGcount;
}



void standShiftInput(int start[], int end[]) {

	int shift = 0;
	string timeinput;
	cout << "Enter the amount of stand shifts you are going to Input:";
	cin >> shift;


	if (shift > 0) {

		for (int i = 0; i < shift; i++) {
			while (true) {
				cout << "Stand Shift start time";
				cin >> timeinput;
				start[i] = findtime(timeinput) - starttime;
				if (start[i] == -1) {
					cout << "Please enter a valid start time" << endl;
				}
				else
					break;
			}
			while (true) {
				cout << "Stand Shift end time:";
				cin >> timeinput;
				end[i] = findtime(timeinput) - starttime;
				if (end[i] == -1) {
					cout << "Please enter a valid start time" << endl;
				}
				else
					break;
			}

		}
	}


	return;
}


void LifeguardShifts(Lifeguard L[], int count, int start[], int end[]) {

	int shift;

	for (int i = 0; i < count; i++) {
		//cout << "Input the shift number for Lifeguard " << i + 1 << ":";
		cin >> shift;


		L[i].starttime = start[shift - 1];
		L[i].endtime = end[shift - 1];
	}


	return;
}


void StandShifts(Stands S[], int count, int start[], int end[]) {

	int shift = -1;
	

	for (int i = 0; i < count; i++) {
		cout << "Input the shift number for the stand " << S[i].name << ":";
		
		cin >> shift;


		S[i].startime = start[shift - 1];
		S[i].endtime = end[shift - 1];
	}




	return;
}




void Print(int LG_count, int time, Lifeguard L[]) {
	for (int y = 0; y < LG_count; y++) {
		for(int x = 0; x < time; x++){
			if (chart[x][y] != "NULL")
				outputfile <<  setw(4) << left << chart[x][y];
			else
				outputfile << setw(4) << "-";
		}
		outputfile << "StandCount:" << L[y].standcounter << endl;
	}


	
}


bool checkStandCount(Lifeguard L[], int count) {

	for (int i = 0; i < count; i++) {
		if (L[i].standcounter != average && L[i].disperse_viable == true)
			return false;
	}



	
	return true;
}


void Disperse(Lifeguard L[], int count, int intervals) {
	int noswap_counter = 0;
	int current_interval = 0;
	int minindex = 0;
	int maxindex = 0;
	int start = -1;
	int end = -1;
	

	while (checkStandCount(L, count) == false && noswap_counter != intervals && loopcounter != 1000) {
		loopcounter++;
		start = -1;
		end = -1;

		for (int i = 0; start == -1; i++) { //find first clocked in Lifeguard and set that as the start
			if (L[i].starttime <= current_interval && L[i].endtime > current_interval && L[i].disperse_viable == true)
				start = i;
		}

		for (int i = 0; i < count; i++) { //find last clocking in Lifeguard and set that as the end
			if (L[i].starttime <= current_interval && L[i].endtime > current_interval && L[i].disperse_viable == true)
				end = i;
		}

		minindex = start;
		maxindex = start;

		for (int i = start; i < end + 1; i++) { // finding min and max


			if (current_interval == 0) { //if they are at beginning of chart you do not need to check if they are previosly down
				if (L[i].standcounter <= L[minindex].standcounter && chart[current_interval][i] == "NULL" && L[i].disperse_viable == true){
					minindex = i;
				}
				if (L[i].standcounter >= L[maxindex].standcounter && chart[current_interval][i] != "NULL" && L[i].disperse_viable == true) {
					maxindex = i;

				}
			}
			else {
				if (L[i].standcounter <= L[minindex].standcounter && chart[current_interval][i] == "NULL" && chart[current_interval - 1][i] == "NULL" && L[i].disperse_viable == true) {
					minindex = i;
				}
				if (L[i].standcounter >= L[maxindex].standcounter && chart[current_interval][i] != "NULL" && L[i].disperse_viable == true) {
					maxindex = i;

				}




			}



		}

		if (L[minindex].standcounter != average && L[maxindex].standcounter != average && minindex != maxindex) {

			chart[current_interval][minindex] = chart[current_interval][maxindex];
			chart[current_interval][maxindex] = "NULL";
			L[minindex].standcounter++;
			L[maxindex].standcounter--;
			noswap_counter = 0;
		}
		else {
			noswap_counter++;
		}

		current_interval++;


		if (current_interval == intervals) {
			current_interval = 0;
		}






	}



}





int main() {

	myfile.open("Stands.txt");
	outputfile.open("rotation.txt");

	srand(time(NULL));
	int poolHours = 0;
	int LifeguardCount = 0;
	int standCount = 0;
	int intervals = 0;
	int random_num = 0;
	bool input = false;
	Lifeguard Guardarray[100];
	Stands StandArray[100];
	int shiftstart[50];
	int shiftend[50];
	int standstart[50];
	int standend[50];
	string timeinput = "NULL";
	bool valid_inputed = false;


	




	
	/*while (input == false) {
		cout << "Enter the amount of Lifeguards:";
		myfile >> LifeguardCount;
		cout << "Enter the amount of stands:";
		myfile >> standCount;

		if (LifeguardCount < standCount) {
			//cout << "current values are impossible there must be more Lifeguards than stands. Please try again" << endl;
		}
		else {
			input = true;
			//cout << "Thank you! Calculating Rotation..." << endl;
		}
	}
	*/

	while (valid_inputed == false) {
		cout << "Enter the opening time of the pool:";
		cin >> timeinput;
		starttime = findtime(timeinput);
		cout << "Enter the closing time of the pool:";
		cin >> timeinput;
		endtime = findtime(timeinput);



		if (starttime < endtime && starttime != -1 && endtime != -1)
			valid_inputed = true;
		else
			cout << "Please enter a valid starting and closing time that follows the format and make sure the closing time is greater than the opening time" << endl;
	}

	poolHours = (endtime - starttime) / 2;
	intervals = poolHours * 2;


	//cout << "Enter the amount of stands:";
	myfile >> standCount;



	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			chart[i][j] = "NULL";
		}
	}

	
	


	StandInput(StandArray, standCount);



	
	
	LifeguardCount = shiftInput(shiftstart, shiftend, Guardarray);

	//LifeguardShifts(Guardarray, LifeguardCount, shiftstart, shiftend);
	
	standShiftInput(standstart, standend);

	StandShifts(StandArray, standCount, standstart, standend);


	


	int index = -1;
	bool Lifeguard_selected = false;
	bool standsLeft = true;
	bool stand_selected = false;
	int remainingStands = 0;
	


	for (int x = 0; x < intervals; x++) { //for every column x
		remainingStands = standCount;
		while (remainingStands != 0) { //while there are stands that need to be taken


			Lifeguard_selected = false;
			
			while (Lifeguard_selected == false) { //choose a random number and check if that random Lifeguard is down and is clocked in
				random_num = rand() % LifeguardCount;
				if (Guardarray[random_num].down == true && Guardarray[random_num].starttime <= x && Guardarray[random_num].endtime > x)
					Lifeguard_selected = true;

				

			
			}

			stand_selected = false;
			while (stand_selected == false) {
				if (StandArray[remainingStands - 1].startime <= x && StandArray[remainingStands - 1].endtime > x)
					stand_selected = true;
				else
					remainingStands--;

			}
			

			chart[x][random_num] = StandArray[remainingStands - 1].name; //give that random Lifeguard the current stand
			Guardarray[random_num].down = false; //that Lifeguard now has a stand so make it not down anymore
			Guardarray[random_num].standcounter++;



			if (x != 0) {

				index = SearchChart(x - 1, StandArray[remainingStands - 1].name, LifeguardCount);
				if (index != -1) {
					Guardarray[index].down = true;
					
				}

			}



			remainingStands--;
		}

	}



	int disperse_viable_counter = 0;


	for (int i = 0; i < LifeguardCount; i++) {
		if (Guardarray[i].disperse_viable == true) {
			average += Guardarray[i].standcounter;
			disperse_viable_counter++;
		}
		
	}
	average = average / disperse_viable_counter;
	



	Print(LifeguardCount, intervals, Guardarray);
	outputfile.close();

	outputfile.open("rotation.txt");
	Disperse(Guardarray, LifeguardCount, intervals);


	
	
	Print(LifeguardCount, intervals, Guardarray);
	outputfile.close();



	return 0;
}