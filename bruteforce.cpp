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

// Function to generate worst-case input for a given number of cities
vector<vector<int>> generateWorstCaseInput(int numCities) {
    vector<vector<int>> distances(numCities, vector<int>(numCities, 0));

    // Set large distances between all cities except the first city
    for (int i = 1; i < numCities; ++i) {
        distances[0][i] = distances[i][0] = 9999;
    }

    return distances;
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
        findShortestRouteBruteForce(bestCaseDistances);
        auto end = high_resolution_clock::now();
        auto durationBestCase = duration_cast<microseconds>(end - start).count();

        // Generate average-case input for the current number of cities
        vector<vector<int>> averageCaseDistances = generateAverageCaseInput(numCities);

        // Brute Force - Average Case
        start = high_resolution_clock::now();
        findShortestRouteBruteForce(averageCaseDistances);
        end = high_resolution_clock::now();
        auto durationAverageCase = duration_cast<microseconds>(end - start).count();

        // Generate worst-case input for the current number of cities
        vector<vector<int>> worstCaseDistances = generateWorstCaseInput(numCities);

        // Brute Force - Worst Case
        start = high_resolution_clock::now();
        findShortestRouteBruteForce(worstCaseDistances);
        end = high_resolution_clock::now();
        auto durationWorstCase = duration_cast<microseconds>(end - start).count();

        cout << "Best Case: " << durationBestCase << " microseconds" << endl;
        cout << "Average Case: " << durationAverageCase << " microseconds" << endl;
        cout << "Worst Case: " << durationWorstCase << " microseconds" << endl;
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
