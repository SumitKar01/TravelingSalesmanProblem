#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <chrono>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using namespace std;
using namespace std::chrono;

// Function to calculate the total distance of a route
int calculateDistance(const vector<int>& route, const vector<vector<int>>& distances) {
    int totalDistance = 0;
    int numCities = route.size();

    for (int i = 0; i < numCities - 1; ++i) {
        int currentCity = route[i];
        int nextCity = route[i + 1];

        totalDistance += distances[currentCity][nextCity];
    }

    // Add the distance from the last city back to the starting city
    totalDistance += distances[route[numCities - 1]][route[0]];

    return totalDistance;
}

// Brute Force Approach
vector<int> findShortestRouteBruteForce(const vector<vector<int>>& distances) {
    int numCities = distances.size();

    // Create a vector with the initial permutation of cities
    vector<int> currentRoute(numCities);
    for (int i = 0; i < numCities; ++i) {
        currentRoute[i] = i;
    }

    // Initialize variables for the shortest route and its distance
    vector<int> shortestRoute = currentRoute;
    int shortestDistance = calculateDistance(currentRoute, distances);

    // Generate all permutations and update the shortest route if necessary
    while (next_permutation(currentRoute.begin(), currentRoute.end())) {
        int currentDistance = calculateDistance(currentRoute, distances);
        if (currentDistance < shortestDistance) {
            shortestDistance = currentDistance;
            shortestRoute = currentRoute;
        }
    }

    return shortestRoute;
}

// Function to generate best-case input for a given number of cities
vector<vector<int>> generateBestCaseInput(int numCities) {
    vector<vector<int>> distances(numCities, vector<int>(numCities, 0));

    // Set equal distances between all cities
    for (int i = 0; i < numCities; ++i) {
        for (int j = i + 1; j < numCities; ++j) {
            distances[i][j] = distances[j][i] = 1;
        }
    }

    return distances;
}

// Function to generate average-case input for a given number of cities
vector<vector<int>> generateAverageCaseInput(int numCities) {
    vector<vector<int>> distances(numCities, vector<int>(numCities, 0));

    // Randomly generate distances between cities
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i != j) {
                distances[i][j] = dist(gen);
            }
        }
    }

    return distances;
}

/// Function to generate worst-case input for a given number of cities
vector<vector<int>> generateWorstCaseInput(int numCities) {
    vector<vector<int>> distances(numCities, vector<int>(numCities, 0));

    // Set large distances between adjacent cities in a linear path
    for (int i = 0; i < numCities - 1; ++i) {
        distances[i][i + 1] = distances[i + 1][i] = 9999;
    }

    // Set a large distance between the last city and the first city
    distances[numCities - 1][0] = distances[0][numCities - 1] = 9999;

    return distances;
}


// Function to print the route
void printRoute(const vector<int>& route) {
    for (int city : route) {
        cout << city << " -> ";
    }
    cout << route[0] << endl;
}

int main() {
    vector<int> numCitiesList;
    vector<long> bestCaseTimings;
    vector<long> averageCaseTimings;
    vector<long> worstCaseTimings;

    for (int numCities = 1; numCities <= 10; ++numCities) {
        cout << "Number of Cities: " << numCities << endl;

        // Generate best-case input for the current number of cities
        vector<vector<int>> bestCaseDistances = generateBestCaseInput(numCities);

        // Brute Force - Best Case
        auto start = high_resolution_clock::now();
        vector<int> bestCaseRoute = findShortestRouteBruteForce(bestCaseDistances);
        auto end = high_resolution_clock::now();
        auto durationBestCase = duration_cast<microseconds>(end - start).count();

        cout << "Best Case: " << durationBestCase << " microseconds" << endl;
        cout << "Shortest Route: ";
        printRoute(bestCaseRoute);
        

        // Generate average-case input for the current number of cities
        vector<vector<int>> averageCaseDistances = generateAverageCaseInput(numCities);

        // Brute Force - Average Case
        start = high_resolution_clock::now();
        vector<int> averageCaseRoute = findShortestRouteBruteForce(averageCaseDistances);
        end = high_resolution_clock::now();
        auto durationAverageCase = duration_cast<microseconds>(end - start).count();

        cout << "Average Case: " << durationAverageCase << " microseconds" << endl;
        cout << "Shortest Route: ";
        printRoute(averageCaseRoute);
        

        // Generate worst-case input for the current number of cities
        vector<vector<int>> worstCaseDistances = generateWorstCaseInput(numCities);

        // Brute Force - Worst Case
        start = high_resolution_clock::now();
        vector<int> worstCaseRoute = findShortestRouteBruteForce(worstCaseDistances);
        end = high_resolution_clock::now();
        auto durationWorstCase = duration_cast<microseconds>(end - start).count();

        cout << "Worst Case: " << durationWorstCase << " microseconds" << endl;
        cout << "Shortest Route: ";
        printRoute(worstCaseRoute);
        cout << endl;

        numCitiesList.push_back(numCities);
        bestCaseTimings.push_back(durationBestCase);
        averageCaseTimings.push_back(durationAverageCase);
        worstCaseTimings.push_back(durationWorstCase);
    }

    // Plot the timings
    plt::plot(numCitiesList, bestCaseTimings, "r-");
    plt::plot(numCitiesList, averageCaseTimings, "g-");
    plt::plot(numCitiesList, worstCaseTimings, "b-");
    plt::title("Execution Time vs. Number of Cities");
    plt::xlabel("Number of Cities");
    plt::ylabel("Execution Time (microseconds)");
    plt::show();

    return 0;
}
