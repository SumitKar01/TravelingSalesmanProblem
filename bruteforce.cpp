#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>


using namespace std;

int numCities;

// Function to calculate the total distance of a tour
double calcTourDistance(const vector<int>& tour, const vector<vector<double>>& distances) {
    double totalDistance = 0.0;
    for (int i = 0; i < numCities - 1; ++i) {
        totalDistance += distances[tour[i]][tour[i + 1]];
    }
    // Add the distance from the last city back to the starting city
    totalDistance += distances[tour[numCities - 1]][tour[0]];
    return totalDistance;
}

// Brute force method to solve the TSP
void tspBruteForce(const vector<vector<double>>& distances) {

    // Create a vector representing the initial tour (0, 1, 2, ..., n-1)
    vector<int> tour(numCities);
    for (int i = 0; i < numCities; i++) {
        tour[i] = i;
    }

    double minDistance = numeric_limits<double>::max();
    vector<int> minTour;

    // Evaluate all possible permutations of the cities
    do {
        double distance = calcTourDistance(tour, distances);
        if (distance < minDistance) {
            minDistance = distance;
            minTour = tour;
        }
    } while (next_permutation(tour.begin() + 1, tour.end()));

    // Output the minimum tour and its distance
    cout << "Minimum Tour: ";
    for (int city : minTour) {
        cout << city << " ";
    }
    cout << endl;
    cout << "Minimum Distance: " << minDistance << endl;

    // Print the cities' coordinates
    // Print the distances between cities
    cout << "Distances between cities:" << endl;
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            cout << "City " << i << " to City " << j << ": " << distances[i][j] << endl;
        }
    }



}

int main() {
    // Example usage
    numCities = 4;

    // Generate random integer coordinates for the cities
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100); // Adjust the range of coordinates as needed

    vector<vector<double>> distances(numCities, vector<double>(numCities));
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i != j) {
                double distance = dist(gen);
                distances[i][j] = distance;
                distances[j][i] = distance;
            }
        }
    }
    tspBruteForce(distances);

    return 0;
}
