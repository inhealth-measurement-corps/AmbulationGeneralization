#include <cstdlib>
#include <iostream>
#include <vector>



class Node {
private:
	std::vector<int> adjacent; 
	std::vector<double> distance;

public:
	friend std::ostream& operator<<(std::ostream& out, const Node& x);

	Node(int sensor = 0, double dis = 0) {
		adjacent.push_back(sensor);
		distance.push_back(dis);
	}

	~Node() {}

	void set_adjacent(int nextsensor, double itsdistance) {
		adjacent.push_back(nextsensor);
		distance.push_back(itsdistance);
	}

	static int const missing_sensor_check = 3; //how many nodes to check ahead searching for the missing sensor before deciding its not in this path.


	vector<int> get_adjacent() { //dont know yet if this should return one element or the entire vector
		return adjacent;
	}


	double get_distance(int sensor) {
		int index = 0;
		double answer;
		for (vector<int>::iterator it = adjacent.begin(); it != adjacent.end();++it) {
			if ((*it) == sensor) {
				answer = distance[index];
				index++;
				return answer;
			}
			index++;
			
		}
	}


	vector<double> get_distance() { //dont know yet if this should return one element or the entire vector
		return distance;
	}
};




