#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>

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

// Greedy Approach
vector<int> findShortestRouteGreedy(const vector<vector<int>>& distances) {
    int numCities = distances.size();

    vector<int> route(numCities);
    for (int i = 0; i < numCities; ++i) {
        route[i] = i;
    }

    int startCity = 0;
    int currentCity = startCity;

    vector<int> shortestRoute = route;
    int shortestDistance = numeric_limits<int>::max();

    do {
        int totalDistance = 0;
        bool validRoute = true;

        for (int i = 0; i < numCities - 1; ++i) {
            int nextCity = route[i + 1];

            if (distances[currentCity][nextCity] == 0) {
                validRoute = false;
                break;
            }

            totalDistance += distances[currentCity][nextCity];
            currentCity = nextCity;
        }

        if (validRoute && distances[currentCity][startCity] != 0) {
            totalDistance += distances[currentCity][startCity];

            if (totalDistance < shortestDistance) {
                shortestDistance = totalDistance;
                shortestRoute = route;
            }
        }

    } while (next_permutation(route.begin() + 1, route.end()));

    return shortestRoute;
}

// Dynamic Programming Approach
vector<int> findShortestRouteDynamic(const vector<vector<int>>& distances) {
    int numCities = distances.size();
    int numSubsets = 1 << numCities;

    vector<vector<int>> dp(numSubsets, vector<int>(numCities, numeric_limits<int>::max()));
    vector<vector<int>> prev(numSubsets, vector<int>(numCities, -1));

    // Initialize base case for subset containing only the starting city
    dp[1][0] = 0;

    // Compute the optimal route and distance for each subset of cities
    for (int subset = 1; subset < numSubsets; ++subset) {
        for (int lastCity = 0; lastCity < numCities; ++lastCity) {
            if ((subset & (1 << lastCity)) != 0) {
                int prevSubset = subset ^ (1 << lastCity);

                for (int currentCity = 0; currentCity < numCities; ++currentCity) {
                    if (lastCity != currentCity && (subset & (1 << currentCity)) != 0) {
                        int newDistance = dp[prevSubset][currentCity] + distances[currentCity][lastCity];

                        if (newDistance < dp[subset][lastCity]) {
                            dp[subset][lastCity] = newDistance;
                            prev[subset][lastCity] = currentCity;
                        }
                    }
                }
            }
        }
    }

    // Find the shortest route by backtracking from the last city
    int lastCity = 0;
    int subset = numSubsets - 1;
    vector<int> shortestRoute(numCities);

    for (int i = numCities - 1; i >= 0; --i) {
        shortestRoute[i] = lastCity;
        int prevCity = prev[subset][lastCity];
        subset ^= (1 << lastCity);
        lastCity = prevCity;
    }

    return shortestRoute;
}

// Helper function to print a vector
void printVector(const vector<int>& vec) {
    for (int val : vec) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    // Example input: distances between cities
    vector<vector<int>> distances = {
        {0, 2, 9, 10},
        {1, 0, 6, 4},
        {15, 7, 0, 8},
        {6, 3, 12, 0}
    };

    // Brute Force
    cout << "Brute Force Approach:" << endl;
    auto startTime = high_resolution_clock::now();
    vector<int> shortestRouteBruteForce = findShortestRouteBruteForce(distances);
    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(endTime - startTime).count();
    cout << "Shortest route: ";
    printVector(shortestRouteBruteForce);
    cout << "Total distance: " << calculateDistance(shortestRouteBruteForce, distances) << endl;
    cout << "Time taken by brute force algorithm: " << duration << " milliseconds" << endl;

    cout << endl;

    // Greedy
    cout << "Greedy Approach:" << endl;
    startTime = high_resolution_clock::now();
    vector<int> shortestRouteGreedy = findShortestRouteGreedy(distances);
    endTime = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(endTime - startTime).count();
    cout << "Shortest route: ";
    printVector(shortestRouteGreedy);
    cout << "Total distance: " << calculateDistance(shortestRouteGreedy, distances) << endl;
    cout << "Time taken by greedy algorithm: " << duration << " milliseconds" << endl;

    cout << endl;

    // Dynamic Programming
    cout << "Dynamic Programming Approach:" << endl;
    startTime = high_resolution_clock::now();
    vector<int> shortestRouteDynamic = findShortestRouteDynamic(distances);
    endTime = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(endTime - startTime).count();
    cout << "Shortest route: ";
    printVector(shortestRouteDynamic);
    cout << "Total distance: " << calculateDistance(shortestRouteDynamic, distances) << endl;
    cout << "Time taken by dynamic programming algorithm: " << duration << " milliseconds" << endl;

    return 0;
}
