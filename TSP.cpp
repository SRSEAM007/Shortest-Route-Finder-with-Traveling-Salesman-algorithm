#include <iostream>
#include <vector>
#include <cmath>         // For pow function to calculate powers
#include <limits>        // For numeric_limits to represent infinity
#include <iomanip>       // For formatting output
#include <algorithm>     // For reverse function

using namespace std;

// Define a large value as infinity for comparisons
const int INF = numeric_limits<int>::max();

/**
 * Function to solve the Traveling Salesman Problem (TSP) using the Dynamic Programming
 * approach (Held-Karp Algorithm). It computes the shortest possible route that visits
 * all locations exactly once and returns to the starting location.
 *
 * @param graph - The distance matrix representing travel costs between locations.
 * @param startNode - The starting location (1-based index).
 * @param bestRoute - A reference to a vector that stores the optimal route.
 * @return The minimum travel cost.
 */
double heldKarp(const vector<vector<double>>& graph, int startNode, vector<int>& bestRoute) {
    int n = graph.size();                   // Number of locations
    int subsetCount = pow(2, n);            // Total subsets (2^n)
    vector<vector<double>> dp(subsetCount, vector<double>(n, INF));

    // Base case: Starting at the startNode, cost is 0
    dp[1 << (startNode - 1)][startNode - 1] = 0;

    // Iterate through all subsets of nodes
    for (int subset = 0; subset < subsetCount; ++subset) {
        for (int i = 0; i < n; ++i) {
            // Skip if 'i' is not in the current subset
            if (!(subset & (1 << i))) continue;

            // Try all possible previous nodes in the subset
            for (int j = 0; j < n; ++j) {
                if (!(subset & (1 << j)) || i == j) continue;

                // Update the cost of reaching 'i' from 'j'
                dp[subset][i] = min(dp[subset][i], dp[subset ^ (1 << i)][j] + graph[j][i]);
            }
        }
    }

    // Find the minimum cost to return to the startNode
    double minDistance = INF;
    int lastNode = -1;  // To track the last node for reconstruction
    for (int i = 0; i < n; ++i) {
        if (i == startNode - 1) continue;  // Skip the startNode
        double cost = dp[subsetCount - 1][i] + graph[i][startNode - 1];
        if (cost < minDistance) {
            minDistance = cost;
            lastNode = i;
        }
    }

    // Reconstruct the optimal route by backtracking
    vector<int> path;
    int subset = subsetCount - 1;
    int currentNode = lastNode;

    // Add the start node at the beginning
    path.push_back(startNode);

    while (currentNode != startNode - 1) {
        path.push_back(currentNode + 1);
        int prevSubset = subset ^ (1 << currentNode);
        for (int i = 0; i < n; ++i) {
            if (i == currentNode || !(subset & (1 << i))) continue;
            if (dp[subset][currentNode] == dp[prevSubset][i] + graph[i][currentNode]) {
                currentNode = i;
                subset = prevSubset;
                break;
            }
        }
    }

    path.push_back(startNode);  // Return to the starting location

    // Reverse the path to get the correct order
    reverse(path.begin(), path.end());

    // Update the bestRoute with the reversed path
    bestRoute = path;

    return minDistance;  // Return the minimum cost
}

/**
 * Utility function to print a line of a specific character.
 *
 * @param ch - The character to print.
 * @param length - The number of times to print the character.
 */
void printLine(char ch, int length) {
    for (int i = 0; i < length; ++i) {
        cout << ch;
    }
    cout << endl;
}

/**
 * Utility function to print the distance matrix.
 *
 * @param matrix - The 2D matrix representing distances between locations.
 */
void printMatrix(const vector<vector<double>>& matrix) {
    cout << "\nDistance Matrix:" << endl;
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            cout << setw(8) << val << " ";
        }
        cout << endl;
    }
}

/**
 * Main function: Handles input, invokes the TSP solver, and displays results.
 */
int main() {
    system("cls"); // Clear the console screen (Windows-specific, optional)

    int n;  // Number of locations
    cout << "Enter the number of locations in the delivery route: ";
    cin >> n;

    // Input the distance matrix
    vector<vector<double>> graph(n, vector<double>(n));
    cout << "Enter the distance matrix (space-separated row-wise):" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << "Enter time for [" << i + 1 << "][" << j + 1 << "]: "; // 1-based indexing for clarity
            cin >> graph[i][j];
        }
    }

    int startNode; // Starting location (1-based index)
    cout << "Enter the starting location (1 to " << n << "): ";
    cin >> startNode;

    // Solve TSP and find the optimal route
    vector<int> bestRoute;
    double minDistance = heldKarp(graph, startNode, bestRoute);

    cout << endl;
    system("cls");

    // Print input summary
    printLine('=', 50);
    cout << setw(30) << "Input Summary" << endl;
    printLine('=', 50);
    printMatrix(graph);

    // Reverse the route before printing
    // reverse(bestRoute.begin(), bestRoute.end());  // Not needed as it's already reversed

    // Print the optimal route and cost
    printLine('=', 50);
    cout << setw(30) << "Optimal Delivery Route" << endl;
    printLine('=', 50);
    cout << "Route: ";
    for (size_t i = 0; i < bestRoute.size(); ++i) {
        cout << bestRoute[i];
        if (i < bestRoute.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;

    // Print each location in the route separately
    cout << "\nLocations in the route (one by one):" << endl;
    for (const int& location : bestRoute) {
        cout << "Location " << location << endl;
    }

    printLine('-', 50);
    cout << "Minimum distance: " << fixed << setprecision(2) << minDistance << " units" << endl;
    printLine('=', 50);

    return 0;
}
