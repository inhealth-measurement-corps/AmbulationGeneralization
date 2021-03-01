#pragma once
#ifndef CONSOLEAPPLICATION1_H
#define CONSOLEAPPLICATION1_H

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <unordered_set>


std::unordered_map<int, Node> create_nodes(map<pair<string, int>, vector<int> >& list);

vector<vector<string> > ambulations(string tracklogfile, string patient, vector<vector<string> >& segments, unordered_map<int, Node> floor, map<pair<string, int>, vector<int> > roomlist);

void sensorcheck(vector<double>& cumulativedistance, vector<int>& sensors, vector<int>& path, vector<int> sensorID, unordered_map<int, Node> floor, int index, map<pair<string, int>, vector<int> > roomlist);

void missingsensorpresent(Node checking, vector<int>& sensors, vector<int>& path, vector<double>& cumulativedistance, int whichsensor, int z, unordered_map<int, Node> floor, vector<int> visited2);
















#endif

//ASSUMPTIONS IN CODE:
/*Valid Input
Ending Sensor is never missed. Last sensor in every path should always be a room sensor.
Shortest path is always calculated.
Ambulation should not take more than 60 minutes
Re-exit of room should be within 60 seconds.
repeating pattern of sensors is assumed to be walking back and forth between them.



*/
