#ifndef TRAVELPLANNER_HPP
#define TRAVELPLANNER_HPP

#include <vector>
#include <string>
#include <map>
#include <utility>

using namespace std;

class TravelPlanner {
private:
    vector<string> locations;
    vector<vector<double>> distances;
    map<string, int> locationToIndex;
    map<int, string> indexToLocation;
    vector<vector<pair<int, double>>> adj;
    int n;

    void dijkstra(int src, vector<double>& dist, vector<int>& parent);
    double calculateTravelTime(double distance);
    string suggestTransportMode(double distance);

public:
    TravelPlanner(vector<string>& locations, vector<vector<double>>& distances);
    
    void shortestPath(string src, string dst);
    void displayFoodPlacesNearLocation(const string& location);
};

#endif
