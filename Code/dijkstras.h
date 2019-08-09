#pragma once
#include "stdafx.h"
#include "targetver.h"
#include <stdio.h>
#include <limits.h>
#include <climits>
#include <iostream>

using namespace std;
#define V 18


/** Dijkstra's Shortest Path Algorithm used to sum distances involving multiple sensors.
* Commonly used when there exists a case of missing sensors. Can also be used to sum multiple
* existing sensors.
* Implemented shortest path for map including all hallway sensors excluding S037 (aka sensor 13.5)
* Note: A translation from original sensor numbering to map numbering must be made (included in main) because
* indices in this algorithm is created using the map going clockwise including sensor S01 (aka sensor 6.5).
* Sensor 1 is S10, 2 is S04, 3 is S013, 4 is S015, .. 18 is S021.
* Parameters:
*	graph: the map represented as a 2D array
*  int fromsensor: starting sensor
*	int tosensor: ending sensor
*	returns double: skipped distance
*/
//helper min function 
int minDistance(double dist[], bool sptSet[])
{
	// Initialize min value
	double min = LONG_MAX;
	int min_index;

	for (int v = 0; v < V; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

void printSolution(double dist[], int n) {
	printf("Vertex   Distance from Source\n");
	for (int i = 0; i < V; i++) {
		printf("%lf \n", dist[i]);
	}
}
//perform dijkstras alg
double dijkstras(double graph[V][V], int src, int tosensor) {
	//translation to this map from original numbering
	if (src > 2) { 
		src++;
	}
	if (tosensor > 2) {
		tosensor++;
	}
	if (src == 101) { //101 denotes sensor 1
		src = 3;
	}
	if (tosensor == 101) {
		tosensor = 3;
	}
	if (src == 0) { //case it is sensor 21
		src = 18;
	}
	if (tosensor == 0) { //case it is sensor 21
		tosensor = 18;
	}
	
	src--; //adjust for array index
	double dist[V];
	bool sptSet[V];
	for (int j = 0; j < V; j++) {
		dist[j] = LONG_MAX;
		sptSet[j] = false;
	}
	dist[src] = 0;
	for (int k = 0; k < V - 1; k++) {
		int u = minDistance(dist, sptSet);
		sptSet[u] = true;
		for (int v = 0; v < V; v++) {
			if (!sptSet[v] && graph[u][v] && dist[u] != LONG_MAX && dist[u] + graph[u][v] < dist[v]) {
				dist[v] = dist[u] + graph[u][v];
			}
		}
	}
	//printSolution(dist, V);
	return dist[tosensor - 1]; //adjust for array index
}

