#include "TravelPlanner.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <climits>
#include <utility>

using namespace std;

TravelPlanner::TravelPlanner(vector<string>& locations, vector<vector<double>>& distances) {
    this->locations = locations;
    this->distances = distances;
    n = locations.size();
    
    // Fill location indices
    for (int i = 0; i < n; ++i) {
        locationToIndex[locations[i]] = i;
        indexToLocation[i] = locations[i];
    }

    // Construct adjacency list
    adj.resize(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (distances[i][j] > 0) {
                adj[i].push_back(make_pair(j, distances[i][j]));
            }
        }
    }
}

// Helper function to make strings case-insensitive
bool caseInsensitiveCompare(const string& a, const string& b) {
    string lowerA = a;
    string lowerB = b;
    transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
    transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
    return lowerA == lowerB;
}

// Function to display food places near a location, with distance and cost estimates
void TravelPlanner::displayFoodPlacesNearLocation(const string& location) {
    map<string, vector<pair<string, double>>> foodPlaces = {
        {"kakadeo", {{"Kakadeo Restaurant", 5.0}, {"Biryani Mahal", 7.0}, {"Momo Gali", 4.0}, {"Jai Mata Di Dhaba", 6.0}}},
        {"jk temple", {{"Temple Cafe", 3.0}, {"Sitaram Kachoriwala", 2.0}, {"Food Corner", 4.0}, {"Sharma Dhaba", 5.0}}},
        {"bithoor", {{"Bithoor Street Food", 3.0}, {"Baba Ki Rasoi", 5.0}, {"Bithoor A1 Dhaba", 6.0}}},
        {"rave moti mall", {{"KFC", 8.0}, {"McDonald's", 7.0}, {"Pizza Hut", 6.5}, {"Haldiram's", 9.0}}},
        {"kanpur zoological park", {{"Zoo Food Court", 5.5}, {"Park View Restaurant", 7.5}, {"Animal Kingdom Cafe", 6.0}}},
        {"vishal mega mart", {{"Food Plaza", 4.5}, {"Haldiram's", 8.0}, {"Chai Sutta Bar", 3.5}, {"The Burger House", 6.5}}},
        {"z square mall", {{"Baskin Robbins", 3.0}, {"Keventers", 3.5}, {"The Coffee Bean & Tea Leaf", 4.5}}},
        {"moti jheel", {{"Moti Jheel Cafe", 4.5}, {"Jheel ki Rasoi", 5.5}, {"Chaiwala", 2.5}, {"Biryani Gali", 6.0}}},
        {"nana rao park", {{"Nana Rao Park Cafe", 3.5}, {"Baba's Restaurant", 6.0}, {"Park View Dhaba", 5.0}}}
    };

    // Convert input location to lowercase
    string input = location;
    transform(input.begin(), input.end(), input.begin(), ::tolower);

    // Attempt to find a close match ignoring case and spaces
    auto it = find_if(foodPlaces.begin(), foodPlaces.end(), [&](const auto& entry) {
        string key = entry.first;
        transform(key.begin(), key.end(), key.begin(), ::tolower);
        return key.find(input) != string::npos || input.find(key) != string::npos;
    });

    if (it != foodPlaces.end()) {
        cout << "\nFood places near " << location << ":\n";
        for (const auto& foodPlace : it->second) {
            double cost = foodPlace.second * 50;
            cout << "- " << foodPlace.first << " (Distance: " << foodPlace.second
                 << " km, Approx Cost: Rs " << cost << ")\n";
        }
    } else {
        cout << "\nNo food places found for " << location << ".\n";
    }
}


// Suggest transport mode based on distance
string TravelPlanner::suggestTransportMode(double distance) {
    if (distance <= 1) {
        return "Walking";
    } else if (distance <= 5) {
        return "Bicycle";
    } else if (distance <= 20) {
        return "Auto Rickshaw";
    } else if (distance <= 50) {
        return "Cab/Taxi";
    } else {
        return "Bus";
    }
}

// Function to calculate the estimated travel time in minutes
double TravelPlanner::calculateTravelTime(double distance) {
    double avgSpeed = 40.0; // Assuming average speed of 40 km/h
    return (distance / avgSpeed) * 60.0; // Convert hours to minutes
}

void TravelPlanner::shortestPath(string src, string dst) {
    // Case insensitive input validation
    auto srcIt = find_if(locations.begin(), locations.end(), [&src](const string& loc) {
        return caseInsensitiveCompare(loc, src);
    });
    auto dstIt = find_if(locations.begin(), locations.end(), [&dst](const string& loc) {
        return caseInsensitiveCompare(loc, dst);
    });

    if (srcIt == locations.end() || dstIt == locations.end()) {
        cout << "Invalid locations. Please check your input.\n";
        return;
    }

    int srcIndex = distance(locations.begin(), srcIt);
    int dstIndex = distance(locations.begin(), dstIt);

    vector<double> dist;
    vector<int> parent;
    dijkstra(srcIndex, dist, parent);

    if (dist[dstIndex] == INT_MAX) {
        cout << "No route found.\n";
        return;
    }

    vector<string> path;
    int current = dstIndex;
    while (current != -1) {
        path.push_back(indexToLocation[current]);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    cout << "\nShortest path from " << src << " to " << dst << ":\n";
    double totalDistance = 0;
    for (int i = 0; i < path.size() - 1; ++i) {
        int u = locationToIndex[path[i]];
        int v = locationToIndex[path[i + 1]];
        totalDistance += distances[u][v];
        cout << path[i] << " -> " << path[i + 1] << ": " << distances[u][v] << " km\n";
    }

    double travelTime = calculateTravelTime(totalDistance);
    string transportMode = suggestTransportMode(totalDistance);

    cout << "\nTotal distance: " << totalDistance << " km\n";
    cout << "Estimated travel time: " << travelTime << " minutes\n";
    cout << "Suggested transport mode: " << transportMode << "\n";

    // Display food places near source and destination
    displayFoodPlacesNearLocation(src);
    displayFoodPlacesNearLocation(dst);
}

void TravelPlanner::dijkstra(int src, vector<double>& dist, vector<int>& parent) {
    dist.assign(n, INT_MAX);
    parent.assign(n, -1);
    dist[src] = 0;

    set<pair<double, int>> pq; // Min-heap based on distance
    pq.insert({0, src});

    while (!pq.empty()) {
        int u = pq.begin()->second;
        double d = pq.begin()->first;
        pq.erase(pq.begin());

        for (const auto& neighbor : adj[u]) {
            int v = neighbor.first;
            double weight = neighbor.second;

            if (dist[u] + weight < dist[v]) {
                pq.erase({dist[v], v});
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.insert({dist[v], v});
            }
        }
    }
}
