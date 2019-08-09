#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>  
#include <sstream>
#include <map>  
#include <thread>
#include <chrono>
#include <cstdlib>
#include "CSVRow.h"
#include "Definements.h"
#include "Node.h"
#include "Reverse.h"
#include "dijkstras.h"
#include "dijkstras2.h"
#include "smart_ptr.h"
#include "targetver.h"
#include <Windows.h> 
#include <Wininet.h>
#include <filesystem>
#include <experimental/filesystem>
#include <tchar.h>
#include <urlmon.h>
#include <ctime>
#include <unordered_map>
#include <utility>
#include "ConsoleApplication1.h"
#include <unordered_set>



#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")


using namespace std;

//Global variables
map<int,vector<int>> tracksensors; //used in missingsensorspresent function.
map<int,vector<double>> trackdistance; //used in missingsensorspresent function.
int counter = 0; //used in missingsensorpresent function.
vector<double> shortestpath; //used in missingsensorpresentfunction.

//list of variables the can be chenged in code:
string floorname = "Floor9.txt";
int length = 6; //how many sensors constitutes a path
int ambulationtime = 3601; //how long an ambulation should be. seconds
int rexit = 60; //if doesn't exit room after 60 seconds mark as new ambulation.
//Node.h::missing_sensor_check controls amount of missing sensors to check for. Currently checks to depth of the value of this variable + 1;




//Function to sort dates
bool sortcol(const vector<string>& v1,
	const vector<string>& v2) {
	return (v1[2] < v2[2]);
}

//function main
int main() {
	//input and CSVtracklog file stuff
	string path;
	cout << "Enter Path/Directory to be analyzed:" << endl;
	cin >> path;

	//ConnectServer connect;
	//econnect.insert(1, 10, "2018-11-15 23:45:30", 200, 12, 2, 1, 1, 0, "0", "2018-10-13 11:10:10");

	vector<vector<string>> patients; //each line of idata file
	CSVRow row;

	//additional file input to create nodes of the floor sensors in the hallway.
	//Right now this being built specifically for use of text file but can be changed.

	map<pair<string, int>, vector<int>> roomlist; //the pair key is the Room number and room sensor. The key of the map allows access to the hallway sensor nearest that room. 
	unordered_map<int, Node> Floormap = create_nodes(roomlist);


	//while (true) { //uncomment this loop when you want to do realtime stuff.
	patients.clear();

	ifstream idata("IData.csv");
	/*ifstream old("database.csv");
	while (old >> row) {

	}*/

	//idata input
	while (idata >> row) {
		vector<string> line;
		if (row[0].empty()) {
			cout << "IData file is empty" << endl;
			break;
		}

		for (int i = 0; i < 4; i++) {
			line.push_back(row[i]);
		}
		patients.push_back(line);
	}
	idata.close();
	sort(patients.begin(), patients.end(), sortcol);

	//Determines the dates that calculations will be done for.
	vector<string> toScanID, toScanStart, toScanEnd, patientID;
	for (int i = patients.size() - 1; i >= 0; i--)
	{
		if (patients[i][2] == "N/A")
		{
			for (int j = i; j >= 0; j--)
			{
				if (patients[i][0] == patients[j][0] && patients[j][3] == "N/A</br>")
				{
					patients[j][3] = patients[i][3];
					patients.erase(patients.begin() + i);
					break;
				}
			}
		}
	}
	for (int i = patients.size() - 1; i >= 0; i--)
	{
		if (patients[i][3] == "N/A</br>")
		{
			toScanID.push_back(patients[i][1]);
			patientID.push_back(patients[i][0]);
			toScanStart.push_back(patients[i][2]);
		}
		else {
			patientID.push_back(patients[i][0]);
			toScanID.push_back(patients[i][1]);
			toScanStart.push_back(patients[i][2]);
			toScanEnd.push_back(patients[i][3]);
		}
	}

	//using the path in order to go through the tracklog files
	stringstream buffer;
	vector<vector<string>> segData;
	vector<vector<string>> livedata; //holds the rows of info spit out every time from temp. The thing is it stores all info for all patients given in idata file. then you need to sort it.
	vector<vector<string>> temp; //each vector here is a row of information for each ambulation on a given day. So 2 ambulations = temp size 2
	
	//goes through the directories and tracklog files

	for (auto& p : experimental::filesystem::recursive_directory_iterator(path)) {
		buffer << p;
		int OPP = buffer.str().find('-') - 4; //Holds integer value of parent directory path
		//series of filters to check whether or not to scan analyze a file by checking name, start date, and end date of file
		if (buffer.str().substr(buffer.str().size() - 5) == ("g.csv"))
		{
				for (int i = 0; i < toScanID.size(); i++) //toScanID correponds to number of patients. Can be replaced with patients vector and nothgng changed in the code
				{
					bool passed = false;
					if (buffer.str().substr(buffer.str().size() - 19, 6) == toScanID[i])
					{
						int comp = toScanStart[i].substr(0, 10).compare(buffer.str().substr(OPP, 10));
						if (toScanEnd.size() >= toScanID.size() && !toScanEnd[i].empty()) { //If there is an end date
							int comp2 = toScanEnd[i].substr(0, 10).compare(buffer.str().substr(OPP, 10));
							if (comp <= 0 && comp2 >= 0) {
								passed = true;
							}
						}
						else if (comp <= 0) {
							passed = true;
						}
						if (passed) // End date functionality added 
						{
							cout << buffer.str() << endl;
							temp = ambulations(buffer.str(), patientID[i], segData, Floormap, roomlist); 
							
							//ambulations: all the ambulations for a given day
							buffer.str(""); //empties buffer
							for (int i = 0; i < temp.size(); i++)
							{
								livedata.push_back(temp[i]);
							}
							//goto output;		// this when doing more than one file. For testing purposes
							break;
						}
					}
				}
			
				
		}
		buffer.str(std::string()); //allows you to empty buffer.
	}

	//output://delete this when doing more than one file

	
	sort(livedata.begin(), livedata.end(), sortcol); //sorts by date

	//output files.

	ofstream output;
	
	output.open("live.csv");
	for (int i = 0; i < livedata.size(); i++)
	{
		if (i > 0 && livedata[i][0] != livedata[i - 1][0]) {
			output << endl;
		}

		for (int j = 0; j < livedata[i].size(); ++j)
		{
			output << livedata[i][j];

			output << ",";
			
		}
		output << endl;
		
	}
	output.close();
	
	output.open("segmentAnalysis.csv");
	for (int i = 0; i < segData.size(); i++)
	{
		if (i > 0 && segData[i][0] != segData[i - 1][0]) {
			output << endl;
		}


		for (int j = 0; j < segData[i].size(); j++)
		{
			output << segData[i][j];

			output << ",";
		
		}
		output << endl;

		
	}
	output.close();



		

		
	cout << "Scan Complete!" << endl;
		
	
	//}
	
	return 0;
}

//map key is the sensorID and the key's value is the node corresponding to that sensor.
//list is a list of all the room sensors with all the nearby hallway sensors. Pair<Romm#, sensorID>,<closest hallway sensors>
unordered_map<int, Node> create_nodes(map<pair<string, int>, vector<int>>& list) {
	unordered_map<int, Node> result;
	ifstream input(floorname);
	double has_next; //to check if there is input left in the file
	int sensorID;
	int adjacent;
	double distance;
	string room = "";
	char c;
	
	while (input >> has_next) {

		sensorID = has_next;
		input >> has_next;
		adjacent = abs(has_next);
		input >> distance;
		input >> c;
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
			input.putback(c);
			if (result.count(sensorID) > 0) {
				result[sensorID].set_adjacent(adjacent, distance);
				goto makeroomlist;
			}
			result[sensorID] = Node(adjacent, distance);
			goto makeroomlist;
		}

		input.putback(c);
		if (result.count(sensorID) > 0) {
			result[sensorID].set_adjacent(adjacent, distance);
			continue;
		}
		result[sensorID] = Node(adjacent, distance);

		
	}
	makeroomlist:
	while (input >> room) {
		input >> sensorID;
		int hallway;
		input >> hallway;
		pair<string, int> temp;
		temp = make_pair(room, sensorID);
		list[temp].push_back(hallway);
		input >> c;
		if (c == ',') {
			while (c == ',') {
				input >> hallway;
				list[temp].push_back(hallway);
				input >> c;
				if (input.eof()) {
					goto close;
				}
			}
			input.putback(c);
		}
		else {
			input.putback(c);
		}
		
	}
	
	close:
	input.close();
	return result;
}

//takes in the sensors data file for a given date, the data of the patients from the idata file, and calcualtes how many ambulations occur on the given date for the patient
	vector<vector<string>> ambulations(string tracklogfile, string patient, vector<vector<string>>& segments, unordered_map<int, Node> floor, map<pair<string, int>, vector<int>> roomlist) {
		vector<vector<string>> output;
		output.clear();
		ifstream tracklog(tracklogfile);
		CSVRow row;
		vector<int> sensorID;
		vector<int> starttime;
		vector<int> endtime;
		int temptime;
		int patientroom = 0; //holds the sensor of patient's room. 

		
		//turns times to seconds and stores them.
		while (tracklog >> row) {
			sensorID.push_back(stoi(row[5]));
			temptime = stoi(row[7].substr(11, 2)) * 3600 + stoi(row[7].substr(14, 2)) * 60 + stoi(row[7].substr(17));
			starttime.push_back(temptime);
			temptime = 0;
			temptime = stoi(row[8].substr(11, 2)) * 3600 + stoi(row[8].substr(14, 2)) * 60 + stoi(row[8].substr(17));
			endtime.push_back(temptime);
		}
		tracklog.close();

		map<int, int> roomfinder;
		roomfinder.clear();

		
		//this is to get rid of any sensors that are not for this floor present in the tracklog file. For now till I found a better way to deal with non paitentroom room sensors.
		
		for (int i = 0; i < sensorID.size();i++) {
			bool found = false;
			if (floor.count(sensorID[i]) == 0) {
				for (map<pair<string, int>, vector<int>>::iterator it = roomlist.begin(); it != roomlist.end(); ++it) {
					if (it->first.second == sensorID[i]) {
						roomfinder[it->first.second]++; //determines which room was detected the most.
						found = true;
						break;
					}

				}
				////This to be commented out if a better way to deal with this is found.
				if (!found) {
					sensorID.erase(sensorID.begin() + i);
					starttime.erase(starttime.begin() + i);
					endtime.erase(endtime.begin() + i);
					i--;
				}
				////
				
			}
		}

		//if 2 sensors or less are left that are of the floor.

		if (sensorID.size() < 3 || starttime.size() < 3) {
			return output;
		}




		int roomcounter = 0;
		for (map<int, int>::iterator it = roomfinder.begin(); it != roomfinder.end(); ++it) {
			if (it->second > roomcounter) {
			patientroom = it->first;
			roomcounter = it->second;
			}
		}


		//file sensors sorting so that I can pass it into sensor check
		map<int,vector<int>> ambulation; //the map to hold the sorted data that will be checked for 6 sensors and missing sensors using sensorcheck.
		int start = 0;
		int index = 0; // to keep track of where we started
		int ambulationcount = 0;


		//delete multiple triggers.
		
			for (int i = 0; i < sensorID.size() - 1; i++) { //this less than keeps from going over size();
		

				if (sensorID[i] == sensorID[i + 1] && starttime[i] == starttime[i + 1]) {
					sensorID.erase(sensorID.begin() + i + 1);
					starttime.erase(starttime.begin() + i + 1);
					endtime.erase(endtime.begin() + i + 1);
					i--;
				}

				else if (sensorID[i] == sensorID[i + 1]) {
				sensorID.erase(sensorID.begin() + i + 1);
				starttime.erase(starttime.begin() + i + 1);
				endtime.erase(endtime.begin() + i + 1);
				i--;
				}
				
			}
		

		vector<int> hallway_sensors;
		if (patientroom != 0) {
			for (map<pair<string, int>, vector<int>>::iterator it = roomlist.begin(); it != roomlist.end(); ++it) {
				if (it->first.second == patientroom) {
					hallway_sensors = it->second;
					break;
				}
			}

			for (int p = 0; p < hallway_sensors.size(); p++) {
				if (floor.count(patientroom) > 0) {
					floor[patientroom].set_adjacent(hallway_sensors[p], 0.1);
					continue;
				}
				floor[patientroom] = Node(hallway_sensors[p], 0.1);

			}	
			
		}




		//creates several possible ambualtions to be checked for missing sensors and if longer than length sensors.

		 
		map<int,vector<int>> ambulation_starttimes;
		map<int, vector<int>> ambulation_endtimes;
		map<int, vector<int>> OOR;
		map<int, vector<int>> OORstimes;
		int OORcount = 0;

		int time_room_to_hallway = 0;
		

		
		//Code Creates the ambulation paths to be checked for 6 sensors. Needs to be tested more but so far works.
		while (index + 1 < sensorID.size()) { //condition to loop through the following code? Works but the problem is haven't been able to test this code enough.


			ambulation[ambulationcount].push_back(sensorID[index]);
			ambulation_starttimes[ambulationcount].push_back(starttime[index]);
			ambulation_endtimes[ambulationcount].push_back(endtime[index]);
			while (starttime[++start] - starttime[index] < ambulationtime) {  //ASSUMPTION:one ambulation should not last more than an hour. time is in seconds. First sensor in file not being checked. FIX.
				

				if (start + 1 == starttime.size()) {
					//if (sensorID[start] == patientroom) {
						ambulation[ambulationcount].push_back(sensorID[start]);
						ambulation_starttimes[ambulationcount].push_back(starttime[start]);
						ambulation_endtimes[ambulationcount].push_back(endtime[start]);
					//}

					break;
				}

				//check for re-exit
				if (sensorID[start] == patientroom) {
					if ((starttime[start + 1] - starttime[start]) > rexit) {
						ambulation[ambulationcount].push_back(sensorID[start]);
						ambulation_starttimes[ambulationcount].push_back(starttime[start]);
						ambulation_endtimes[ambulationcount].push_back(endtime[start]);
						break; 
					}

			
				}
				/* An idea of how to potnetially better deal with other floor sensors by everytime I find one end the current ambulation and while it stays other floor place in OOR map then when I get
				back to the right floor again resuem with ambulation. However does not completely work yet.
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//to use this had to comment out line that gets rid of all non hallway/room sensors above.
				if (sensorID[start] != floor.count(sensorID[start]) && (sensorID[start] != patientroom)) {
					bool find2 = false;
						for (map<pair<string, int>, vector<int>>::iterator it = roomlist.begin(); it != roomlist.end(); ++it) {
							if (it->first.second == sensorID[start]) {
								find2 = true;
									break;
							}
						}
					if (!find2) {
						OOR[OORcount].push_back(sensorID[start]);
						OORstimes[OORcount].push_back(starttime[start]);
						continue;
					}

				}

				//trying to improve ending determination
				if (sensorID[start + 1] != floor.count(sensorID[start + 1]) && (sensorID[start + 1] != patientroom)) {
					bool find = false;
					for (map<pair<string, int>, vector<int>>::iterator it = roomlist.begin(); it != roomlist.end(); ++it) {
						if (it->first.second == sensorID[start + 1]) {
							find = true;
							break;
						}
					}
					if (!find) {
						ambulation[ambulationcount].push_back(sensorID[start]);
						ambulation_starttimes[ambulationcount].push_back(starttime[start]);
						ambulation_endtimes[ambulationcount].push_back(endtime[start]);
						break;
					}

				}

				///////////////////////////////////////////////////////////////
				*/


				ambulation[ambulationcount].push_back(sensorID[start]);
				ambulation_starttimes[ambulationcount].push_back(starttime[start]);
				ambulation_endtimes[ambulationcount].push_back(endtime[start]);

			} //end of while determing ambulation

			//if 2 or less sensors delete ambulation. 
			if (ambulation[ambulationcount].size() < 3) {
				ambulation.erase(ambulationcount);
				//map<int, pair<int, int>>::iterator it = ambulation_time_frames.find(ambulationcount);
				ambulation_starttimes.erase(ambulationcount);
				ambulation_endtimes.erase(ambulationcount);
				
				goto next;
			}

			
			//if end room sensor isn't present in the ambulation.
			if (ambulation[ambulationcount][ambulation[ambulationcount].size() - 1] != patientroom) {
				ambulation[ambulationcount].push_back(patientroom);
				ambulation.erase(ambulationcount); //this line and the one other assume that the ending sensor is always preent in the ambulation.
				goto next;//This assumption is from the files I dealt with. If it's found to not be the case, then comment out these 2 commented lines.
			}

			//In order to calculate time starting from first hallway sensor.
			if (ambulation[ambulationcount].front() == patientroom) {
				time_room_to_hallway = ambulation_starttimes[ambulationcount][0]; //can use this variable for information later instead of throwing away.
				ambulation_starttimes[ambulationcount].erase(ambulation_starttimes[ambulationcount].begin());
			}

			if (ambulation[ambulationcount].front() != patientroom) {
				ambulation[ambulationcount].insert(ambulation[ambulationcount].begin(), patientroom);
			}

			//delete any ambulations that have room sensors in the middle that aren't hallway sensors or patient room.
			//this needs to be modified to do something with the nonpatient room sensors instead of deleting
			//This can be changed to output them as OOR?? To account for the room can use same strategy that I use for the patient's room?? Make node of it, set the distances to 0.1, and use it as any other node?
			for (int j = 0; j < ambulation[ambulationcount].size(); j++) {
				if (floor.count(ambulation[ambulationcount][j]) == 0 && ambulation[ambulationcount][j] != patientroom) {   //if not hallway sensor or patient room sensor

				
					
					ambulation.erase(ambulationcount);
					ambulation_starttimes.erase(ambulationcount);
					ambulation_endtimes.erase(ambulationcount);

					break;
				}
			}

		next:
			ambulationcount++;
			index = start; //I check where I leave off.

		}
		

		

		string roomID;
		for (map<pair<string, int>, vector<int>>::iterator it = roomlist.begin(); it != roomlist.end(); ++it) {
			if (it->first.second == patientroom) {
				roomID = it->first.first;
				break;
			}
		}

		
		vector<int> missingsensors;
		vector<int> path;
		vector<double> cumulativedistance;
		
		vector<double> storespeed;

		vector<string> temp;
		string date = tracklogfile.substr(59, 10);
		string number = tracklogfile.substr(70, 6);
		
		//runs sensorcheck on each ambulation remaining and puts result in segment variable to be outputted later.

		for (int i = 0; i < ambulationcount; i++) {
			bool failed = false;

			if (ambulation.count(i) == 0) {
				continue;
			}
			missingsensors.clear();
			path.clear();
			cumulativedistance.clear();

			sensorcheck(cumulativedistance, missingsensors, path, ambulation[i], floor, 0, roomlist); //can use it to account for if 6 sensors were passed and for trajectory. But sensorID needs to be sorted before passing into here. Maybe on the basis of time. Need to determine which paths are worth checking and then call once for each path.
			
			///////////////added this condiiton but hasn't been triggered yet. If any problems comment out.
			if (path[path.size() - 1] == 0) {
				for (int j = 0; j < ambulation[i].size(); j++) {
					temp.push_back(to_string(ambulation[i][j]));
				}
				temp.push_back("Path could not be completed;A next Sensor was not found"); //either not a path or missing_sensor_check needs to be increased.
				segments.push_back(temp);
				temp.clear();
				continue;
			}
			/////////////

			if (path.size() < length) {
				failed = true;
			}

			for (int z = 0; z < cumulativedistance.size(); z++) {
				if (cumulativedistance[z] == 0.1) { //means room sensor distance meant to be thrown. 
					cumulativedistance.erase(cumulativedistance.begin() + z);
					z--;
				}
			}


			int ind = 1;
			double sum = 0;
			int time = 0; //this is the cumulative time.
			storespeed.clear();
			int speedtime = 0;
			
			for (int k = 0; k < path.size(); k++) {
				bool missed = false;

				temp.push_back(date);
				temp.push_back(patient);
				temp.push_back(number);
				temp.push_back(roomID); 
				temp.push_back(to_string(path[k]));
				if (ind < ambulation[i].size() && path[k] == ambulation[i][ind]) { //ind because i'm not including room sensor. and path is larger.
					int hours = ambulation_starttimes[i][ind - 1] / 3600 % 60;
					int min = ambulation_starttimes[i][ind -1] / 60 % 60;
					int sec = ambulation_starttimes[i][ind - 1] % 60;
					string formattime = to_string(hours) + ":" + to_string(min) + ":" + to_string(sec);
					temp.push_back(formattime); //push back the actual time.
					time =  ambulation_starttimes[i][ind - 1] - ambulation_starttimes[i][0]; //use the cumulative time.
						temp.push_back(to_string(time)); //current time in seconds. subtracted from starting time.
						
							speedtime = ambulation_starttimes[i][ind - 1] - ambulation_starttimes[i][ind - 2]; // this is the time to use for the segspeed for now.
						
						ind++;
						
				}
					
				else {
					temp.push_back("Missed Sensor 00:00:00");
						temp.push_back(to_string(time)); 
						missed = true;
					
				}

				if (k <= cumulativedistance.size() && k > 0) { //double check the = I put here.
					
					sum += cumulativedistance[k - 1];
					temp.push_back(to_string(sum));
					


					if (speedtime != 0 && !missed) {
						double segspeed = cumulativedistance[k] / (double) speedtime;
						storespeed.push_back(segspeed);
						temp.push_back(to_string(segspeed));
					}

					if (missed) {
						temp.push_back("Segment speed not available using this sensor");
					}
					
				}

				else {
					temp.push_back(to_string(sum));
					temp.push_back("Segment Speed not available using this sensor");
				}

			
				
				if (failed) {
					temp.push_back("Out of room");

				}

				else {
					temp.push_back("Ambulation");
				}

				segments.push_back(temp);
				temp.clear();
			}
			



			temp.clear();

			if (!failed) { //meaning successful ambulation
				temp.push_back(date);
				temp.push_back(patient);
				temp.push_back(number);

				int hours = ambulation_starttimes[i][0] / 3600 % 60;
				int min = ambulation_starttimes[i][0] / 60 % 60;
				int sec = ambulation_starttimes[i][0] % 60;
				string formattime = to_string(hours) + ":" + to_string(min) + ":" + to_string(sec);
				temp.push_back(formattime);

				temp.push_back(to_string(sum));
				double avgspeed = 0;
				int tottime = 0;
				tottime = ambulation_endtimes[i][ambulation_endtimes[i].size() - 1] - ambulation_starttimes[i][0];

				avgspeed = sum / tottime;

				temp.push_back(to_string(avgspeed));
				temp.push_back(to_string((ambulation_endtimes[i][ambulation_endtimes[i].size() - 1] - ambulation_starttimes[i][0])));

				string temp3;
				temp3.clear();
				
				//stores all sensors missed on that ambulation.
				for (int i = 0; i < missingsensors.size(); i++) {
					temp3 += to_string(missingsensors[i]);
					temp3 += " ";
				}
				temp.push_back(temp3);
				output.push_back(temp);
				temp.clear();

			}
			
			
		}


		
		return output;
	}


	//checks for next sensor along the path recursively. Doesn't find it means missing sensor and goes to missingsensorpresent. 

	void sensorcheck(vector<double>& cumulativedistance, vector<int>& sensors, vector<int>& path, vector<int> sensorID, unordered_map<int, Node> floor, int index, map<pair<string, int>, vector<int>> roomlist) {

		vector<int> visited2;
		

		bool found = false;
		int i = index;
		/*if (i == 0) {
			path.push_back(sensorID[i]);
		}*/

		if (i + 1 == sensorID.size()) {
			//path.push_back(sensorID[i]);
			return;
		}

		unordered_map<int, Node>::iterator it = floor.find(sensorID[i]);

		
		//dealing with end sensor
		
		if (i + 1 == sensorID.size() - 1) {																			//&& is patient room?
			unordered_map<int, Node>::iterator it = floor.find(sensorID[i + 1]);

			
				for (int p = 0; p < it->second.get_adjacent().size(); p++) {
					if (sensorID[i] == it->second.get_adjacent()[p]) {
						path.push_back(sensorID[i + 1]); //dont want to add the ending room sensor.
						goto present;
					}
				}
				

					trackdistance.clear();
					tracksensors.clear();
					visited2.clear();
					shortestpath.clear();
					visited2.push_back(it->first);
					counter = 0;
					missingsensorpresent(it->second, sensors, path, cumulativedistance, sensorID[i], 0, floor, visited2); //this needs to be altered and intertwined with general djistrikas


					if (counter > 0) {

						double temp2 = INFINITY;
						int in = 0;

						

						
						for (int i = 0; i < shortestpath.size(); i++) {
							if (shortestpath[i] < temp2) {
								in = i;
								temp2 = shortestpath[i];
							}
							////if two distances are the same output the one that triggered more sensors. uncomment this if that is what you want. Otherwise it will do least sensors.
							/*
							else if (shortestpath[i] == temp2) {
								if (tracksensors[i].size() > tracksensors[in].size()) {
									in = i;
									temp2 = shortestpath[i];
								}
							}
							*/
						}

						for (int j = tracksensors[in].size() - 2; j >= 0; j--) {

							path.push_back(tracksensors[in][j]);
							sensors.push_back(tracksensors[in][j]);
						}

						for (int i = trackdistance[in].size() - 1; i >= 0; i--) {
							cumulativedistance.push_back(trackdistance[in][i]);
						}

						sensors.pop_back();
						//path.pop_back();


					}

					else {
					
						path.push_back(0);
						return;
										
					}

				

				goto present;
			


		}

		//dealing with end sensor


			for (int j = 0; j < it->second.get_adjacent().size(); j++) {

				if (sensorID[i + 1] == it->second.get_adjacent()[j]) {
					path.push_back(it->second.get_adjacent()[j]);
					cumulativedistance.push_back(it->second.get_distance()[j]);
					goto present;
				}
			}

					trackdistance.clear();
					tracksensors.clear();
					visited2.clear();
					shortestpath.clear();
					visited2.push_back(it->first);
					counter = 0;
					missingsensorpresent(it->second, sensors, path, cumulativedistance, sensorID[i + 1], 0, floor, visited2); //this needs to be altered and intertwined with general djistrikas

					/*if (tracksensors.empty()) {
						cout<<"No sensor found. May need to adjust missing_sensor_check or check inputdata"<<endl;
					}*/
					
					
					if (counter > 0) { //counter seems like its always greater than zero so how do i ever go to the if statement?

						double temp2 = INFINITY;
						int in = 0;




						for (int i = 0; i < shortestpath.size(); i++) {
							if (shortestpath[i] < temp2) {
								in = i;
								temp2 = shortestpath[i];
							}
								////if two distances are the same output the one that triggered more sensors
								/*
								else if (shortestpath[i] == temp2) {
								if (tracksensors[i].size() > tracksensors[in].size()) {
								in = i;
									temp2 = shortestpath[i];
								}
								}
								*/
						}

						for (int j = 1; j < tracksensors[in].size(); j++) {

							path.push_back(tracksensors[in][j]);
							sensors.push_back(tracksensors[in][j]);
						}

						for (int i = 0; i < trackdistance[in].size(); i++) {
							cumulativedistance.push_back(trackdistance[in][i]);
						}

						sensors.pop_back();
						
						
					}

					else { //it found nothing. I'm saying if it doesn't find any missingsensors that means that path is too far too connect or missingsensorcheck isn't big enough.
						path.push_back(0);
						return;
					}
		
		
				present:
					sensorcheck(cumulativedistance, sensors, path, sensorID, floor, ++i, roomlist);

	}

	
	
	

	//uses 4 global varaibles check the top of the code.
	//depth-frst traversal using recursion in order to find all possible paths. Depth is controlled by variable missing_sensor_check in Node.h. The first Node checking passed in from 
	//sensorcheck passes in the first node which is depth 0. Currently checks to a depth of missing_sensor_check + 1 aka if check variable is 3 it will see up to 4 sensors were missed in a row.
	void missingsensorpresent(Node checking, vector<int>& sensors, vector<int>& path, vector<double>& cumulativedistance, int whichsensor, int z, unordered_map<int, Node> floor, vector<int> visited2) {
		vector<int> visited = visited2;

		//This first for loop only executes when the sensor we are looking for (whichsensor) is found. 
		//double temp = 0;
		for (int k = 0; k < checking.get_adjacent().size(); k++) {
			if (whichsensor == checking.get_adjacent()[k]) { // I know its not in the first node coming in's adjacent because its already been checked. Done this way so the recursion can work right. If I check then,this would end and putting at end would make recursion take longer.
				for (int i = 0; i < visited.size(); i++) {
					tracksensors[counter].push_back(visited[i]);
				}
				tracksensors[counter].push_back(checking.get_adjacent()[k]);

				for (int i = 0; i < tracksensors[counter].size(); i++) {
					unordered_map<int, Node>::iterator it = floor.find(tracksensors[counter][i]);
					if (i + 1 < tracksensors[counter].size()) {
						trackdistance[counter].push_back(it->second.get_distance(tracksensors[counter][i + 1]));
					}
				}

				double temp = 0;
				for (int i = 0; i < trackdistance[counter].size(); i++) {//addded
					temp += trackdistance[counter][i];
				}

				shortestpath.push_back(temp);//added
				

				counter++;
				return;
			}
		}

				for (int k = 0; k < checking.get_adjacent().size(); k++) {


					for (int w = z; w < Node::missing_sensor_check; ) {
						for (vector<int>::iterator it = visited.begin(); it != visited.end(); ++it) { //see if this could be improved somehow because I Keep searching through vector so much.
							if (*it == checking.get_adjacent()[k]) { //this should be fine but there are any errors check here.
								w++;
								break;
							}
						}
						if (w > z) {
							visited.push_back(checking.get_adjacent()[k]); //im putting something I know I will delete. See if i can change this after it works
							break;
						}
						visited.push_back(checking.get_adjacent()[k]);
						unordered_map<int, Node>::iterator it = floor.find(checking.get_adjacent()[k]);
						missingsensorpresent(it->second, sensors, path, cumulativedistance, whichsensor, ++w, floor, visited);

						break;
					}
					if (!(z < Node::missing_sensor_check)) {
						return; //or break
					}
					if (!visited.empty()) {
						visited.pop_back();
												//remove last node.
					}

					// removes the parent node's child from visited so that it isn't present in the path of a different adjacent node.
					if (z == 1 && (k + 1 == checking.get_adjacent().size())) {
						visited.pop_back();
					}
					//
				}

	}
	