#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_RECORDS 10

// Record structure
typedef struct Record {
    int id;
    int importance; // Importance of the record
} Record;

// Graph representing relationships between records
int graph[MAX_RECORDS][MAX_RECORDS];
int visited[MAX_RECORDS];
int importance[MAX_RECORDS];
int distance[MAX_RECORDS]; // Array to store the shortest distance to each record

// Initialize the graph
void initGraph() {
    for (int i = 0; i < MAX_RECORDS; i++) {
        visited[i] = 0;
        importance[i] = 0;
        distance[i] = INT_MAX; // Initialize shortest distance array
        for (int j = 0; j < MAX_RECORDS; j++) {
            graph[i][j] = 0;
        }
    }
}

// Add an edge to the graph
void addEdge(int source, int destination, int weight) {
    graph[source][destination] = weight;
    // If the graph is undirected, add an edge from destination to source as well
    graph[destination][source] = weight;
}

// Visit records starting from the most important one using Dijkstra's algorithm
void dijkstra(int start) {
    distance[start] = 0; // The shortest distance to the start node is 0

    for (int count = 0; count < MAX_RECORDS - 1; count++) {
        int maxIndex = -1;
        int maxImportance = INT_MIN;
        for (int j = 0; j < MAX_RECORDS; j++) {
            if (!visited[j] && importance[j] > maxImportance) {
                maxImportance = importance[j];
                maxIndex = j;
            }
        }

        if (maxIndex == -1)
            break;

        visited[maxIndex] = 1;

        for (int j = 0; j < MAX_RECORDS; j++) {
            if (graph[maxIndex][j] && distance[maxIndex] != INT_MAX &&
                distance[maxIndex] + graph[maxIndex][j] < distance[j]) {
                distance[j] = distance[maxIndex] + graph[maxIndex][j];
            }
        }
    }
}

// Function to return the shortest distances to each record
int* shortestDistances() {
    return distance;
}

int main() {
    initGraph();

    // Add relationships between records to the graph
    addEdge(0, 1, 5);
    addEdge(0, 2, 3);
    addEdge(1, 3, 6);
    addEdge(2, 4, 2);
    addEdge(3, 5, 7);
    addEdge(4, 6, 4);
    addEdge(5, 7, 3);
    addEdge(6, 8, 5);

    // Set the importance of each record
    importance[0] = 10;
    importance[1] = 5;
    importance[2] = 8;
    importance[3] = 6;
    importance[4] = 4;
    importance[5] = 7;
    importance[6] = 3;
    importance[7] = 2;
    importance[8] = 1;

    // Perform query optimization using Dijkstra's algorithm
    printf("Shortest distances to each record starting from Record 0:\n");
    dijkstra(0);

    // Print the shortest distance to each record
    int* distances = shortestDistances();
    for (int j = 0; j < MAX_RECORDS; j++) {
        if (distances[j] != INT_MAX) {
            printf("Record %d: %d\n", j, distances[j]);
        }
    }

    return 0;
}
