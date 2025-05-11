#include <iostream>
#include <vector>
#include "TravelPlanner.hpp"

using namespace std;


void displayAvailablePlaces() {
    cout << "Available Places in Kanpur:\n";
    vector<string> places = {
        "Kakadeo", "JK Temple", "Bithoor", "Rave Moti Mall", "Kanpur Zoological Park",
        "Vishal Mega Mart", "Z Square Mall", "Moti Jheel", "Nana Rao Park"
    };
    for (const auto& place : places) {
        cout << "- " << place << "\n";
    }
    cout << "\n";
}

int main() {
    vector<string> locations = {
        "Kakadeo", "JK Temple", "Bithoor", "Rave Moti Mall", "Kanpur Zoological Park",
        "Vishal Mega Mart", "Z Square Mall", "Moti Jheel", "Nana Rao Park"
    };

    vector<vector<double>> distances = {
        {0, 5, 20, 8, 6, 5, 9, 7, 4},
        {5, 0, 25, 10, 7, 6, 8, 6, 3},
        {20, 25, 0, 30, 27, 26, 29, 24, 22},
        {8, 10, 30, 0, 5, 3, 6, 8, 7},
        {6, 7, 27, 5, 0, 4, 7, 5, 6},
        {5, 6, 26, 3, 4, 0, 6, 6, 7},
        {9, 8, 29, 6, 7, 6, 0, 4, 5},
        {7, 6, 24, 8, 5, 6, 4, 0, 3},
        {4, 3, 22, 7, 6, 7, 5, 3, 0}
    };

    TravelPlanner planner(locations, distances);

    displayAvailablePlaces(); // Show suggestions

    string source, destination;
    cout << "Enter source location: ";
    getline(cin, source);
    cout << "Enter destination location: ";
    getline(cin, destination);

    planner.shortestPath(source, destination);

    return 0;
}
