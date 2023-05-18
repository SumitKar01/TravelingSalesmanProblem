#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using namespace std;

// Function to calculate the Euclidean distance between two cities
double calcDistance(pair<int, int> city1, pair<int, int> city2) {
    double xDiff = city1.first - city2.first;
    double yDiff = city1.second - city2.second;
    return sqrt(xDiff * xDiff + yDiff * yDiff);
}

// Function to calculate the total distance of a tour
double calcTourDistance(const vector<int>& tour, const vector<pair<int, int>>& cities, int numCities) {
    double totalDistance = 0.0;
    for (int i = 0; i < numCities - 1; ++i) {
        totalDistance += calcDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    // Add the distance from the last city back to the starting city
    totalDistance += calcDistance(cities[tour[numCities - 1]], cities[tour[0]]);
    return totalDistance;
}
// Plot the cities and the optimal tour
void plotResults(const vector<pair<int, int>>& cities, int numCities, vector<int> minTour) {
    vector<double> xCoords, yCoords;
    for (int i = 0; i < numCities; ++i) {
        plt::text(cities[i].first, cities[i].second, to_string(i));
    }
    plt::plot(xCoords, yCoords, "bo");

    vector<double> optXTour, optYTour;
    for (int city : minTour) {
        optXTour.push_back(cities[city].first);
        optYTour.push_back(cities[city].second);
    }
    optXTour.push_back(cities[minTour[0]].first);
    optYTour.push_back(cities[minTour[0]].second);
    plt::plot(optXTour, optYTour, "r-");

    plt::show();
}
// Brute force method to solve the TSP
void tspBruteForce(const vector<pair<int, int>>& cities, int numCities) {

    // Create a vector representing the initial tour (0, 1, 2, ..., n-1)
    vector<int> tour(numCities);
    for (int i = 0; i < numCities; i++) {
        tour[i] = i;
    }

    double minDistance = numeric_limits<double>::max();
    vector<int> minTour;

    // Evaluate all possible permutations of the cities
    do {
        double distance = calcTourDistance(tour, cities, numCities);
        if (distance < minDistance) {
            minDistance = distance;
            minTour = tour;
        }
    } while (next_permutation(tour.begin() + 1, tour.end()));

    // Output the optimal tour and its distance
    cout << "Optimal Tour: ";
    for (int city : minTour) {
        cout << city << " ";
    }
    cout << endl;
    cout << "Optimal Distance: " << minDistance << endl;

    // Print the cities' coordinates
    cout << "Cities' Coordinates:" << endl;
    for (int i = 0; i < numCities; ++i) {
        cout << "City " << i << ": (" << cities[i].first << ", " << cities[i].second << ")" << endl;
    }

    plotResults(cities, numCities, minTour);


}

int main() {
    // Example usage
    int numCities;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    // Generate random integer coordinates for the cities
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 10); // Adjust the range of coordinates as needed

    vector<pair<int, int>> cities(numCities);
    for (int i = 0; i < numCities; ++i) {
        cities[i].first = dist(gen);
        cities[i].second = dist(gen);
    }

    tspBruteForce(cities, numCities);

    return 0;
}
