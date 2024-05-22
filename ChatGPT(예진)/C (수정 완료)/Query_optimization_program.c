#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_RECORDS 10

// ���ڵ� ����ü
typedef struct Record {
    int id;
    int importance; // ���ڵ��� �߿䵵
} Record;

// ���ڵ� ���� ���踦 ��Ÿ���� �׷���
int graph[MAX_RECORDS][MAX_RECORDS];
int visited[MAX_RECORDS];
int importance[MAX_RECORDS];
int distance[MAX_RECORDS]; // �� ���ڵ������ �ִ� �Ÿ��� �����ϴ� �迭

// �׷��� �ʱ�ȭ
void initGraph() {
    for (int i = 0; i < MAX_RECORDS; i++) {
        visited[i] = 0;
        importance[i] = 0;
        distance[i] = INT_MAX; // �ִ� �Ÿ� �迭 �ʱ�ȭ
        for (int j = 0; j < MAX_RECORDS; j++) {
            graph[i][j] = 0;
        }
    }
}

// �׷����� ���� �߰�
void addEdge(int source, int destination, int weight) {
    graph[source][destination] = weight;
    // ������ ���� �׷������, destination���� source�� ���� ������ �߰��մϴ�.
    graph[destination][source] = weight;
}

// ����ũ��Ʈ�� �˰����� ����Ͽ� ���� �߿��� ���ڵ���� �湮
void dijkstra(int start) {
    distance[start] = 0; // ���� ����� �ִ� �Ÿ��� 0

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

// �� ���ڵ������ �ִ� �Ÿ��� ��ȯ�ϴ� �Լ�
int* shortestDistances() {
    return distance;
}

int main() {
    initGraph();

    // ���ڵ� ���� ���踦 �׷����� �߰�
    addEdge(0, 1, 5);
    addEdge(0, 2, 3);
    addEdge(1, 3, 6);
    addEdge(2, 4, 2);
    addEdge(3, 5, 7);
    addEdge(4, 6, 4);
    addEdge(5, 7, 3);
    addEdge(6, 8, 5);

    // ���ڵ��� �߿䵵 ����
    importance[0] = 10;
    importance[1] = 5;
    importance[2] = 8;
    importance[3] = 6;
    importance[4] = 4;
    importance[5] = 7;
    importance[6] = 3;
    importance[7] = 2;
    importance[8] = 1;

    // ����ũ��Ʈ�� �˰����� ����Ͽ� ���� ����ȭ ����
    printf("Shortest distances to each record starting from Record 0:\n");
    dijkstra(0);

    // �� ���ڵ������ �ִ� �Ÿ��� ���
    int* distances = shortestDistances();
    for (int j = 0; j < MAX_RECORDS; j++) {
        if (distances[j] != INT_MAX) {
            printf("Record %d: %d\n", j, distances[j]);
        }
    }

    return 0;
}
