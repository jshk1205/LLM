#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_RECORDS 10

// 레코드 구조체
typedef struct Record {
    int id;
    int importance; // 레코드의 중요도
} Record;

// 레코드 간의 관계를 나타내는 그래프
int graph[MAX_RECORDS][MAX_RECORDS];
int visited[MAX_RECORDS];
int importance[MAX_RECORDS];
int distance[MAX_RECORDS]; // 각 레코드까지의 최단 거리를 저장하는 배열

// 그래프 초기화
void initGraph() {
    for (int i = 0; i < MAX_RECORDS; i++) {
        visited[i] = 0;
        importance[i] = 0;
        distance[i] = INT_MAX; // 최단 거리 배열 초기화
        for (int j = 0; j < MAX_RECORDS; j++) {
            graph[i][j] = 0;
        }
    }
}

// 그래프에 간선 추가
void addEdge(int source, int destination, int weight) {
    graph[source][destination] = weight;
    // 방향이 없는 그래프라면, destination에서 source로 가는 간선도 추가합니다.
    graph[destination][source] = weight;
}

// 다이크스트라 알고리즘을 사용하여 가장 중요한 레코드부터 방문
void dijkstra(int start) {
    distance[start] = 0; // 시작 노드의 최단 거리는 0

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

// 각 레코드까지의 최단 거리를 반환하는 함수
int* shortestDistances() {
    return distance;
}

int main() {
    initGraph();

    // 레코드 간의 관계를 그래프에 추가
    addEdge(0, 1, 5);
    addEdge(0, 2, 3);
    addEdge(1, 3, 6);
    addEdge(2, 4, 2);
    addEdge(3, 5, 7);
    addEdge(4, 6, 4);
    addEdge(5, 7, 3);
    addEdge(6, 8, 5);

    // 레코드의 중요도 설정
    importance[0] = 10;
    importance[1] = 5;
    importance[2] = 8;
    importance[3] = 6;
    importance[4] = 4;
    importance[5] = 7;
    importance[6] = 3;
    importance[7] = 2;
    importance[8] = 1;

    // 다이크스트라 알고리즘을 사용하여 쿼리 최적화 수행
    printf("Shortest distances to each record starting from Record 0:\n");
    dijkstra(0);

    // 각 레코드까지의 최단 거리를 출력
    int* distances = shortestDistances();
    for (int j = 0; j < MAX_RECORDS; j++) {
        if (distances[j] != INT_MAX) {
            printf("Record %d: %d\n", j, distances[j]);
        }
    }

    return 0;
}
