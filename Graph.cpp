#include <vector>
#include <unordered_map>

class SensorNode {
private:
	int sensorID;
	std::vector<Adjacency> neighbor_list;
public:
	//to help with traversals if needed
	bool visited = false;
	int sensorID() { return sensorID; }
	std::vector<Adjacency> neighbors() { return neighbor_list; }
	SensorNode(int sensorID) : sensorID(sensorID), neighbor_list(neighbor_list()) { }
	//void addNeighbor(SensorNode* node)
};

class Adjacency {
private:
	SensorNode* neighbor;
	double weight;
public:
	Adjacency(SensorNode* node, double weight) : neighbor(node), weight(weight) { }
};

class Graph {
private:
	//getting the sensor associated with a sensorID could be a common operation
	std::unordered_map<int, SensorNode> nodes;
};

//dense graph representation uses edge matrix instead of adjacency list
class DenseGraph {

};