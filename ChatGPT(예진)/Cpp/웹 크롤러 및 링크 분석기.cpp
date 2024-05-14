// rank 계산이 되지 않음 (그래서 프로그램 종료가 안 됨)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_URL_LENGTH 1024
#define MAX_PAGES 5
#define DAMPING_FACTOR 0.85
#define EPSILON 0.0001

// 그래프 노드를 나타내는 구조체
typedef struct Node {
    char url[MAX_URL_LENGTH];
    struct Node* edges[MAX_PAGES];
    int num_edges;
    float page_rank;
} Node;

// 그래프를 나타내는 구조체
typedef struct Graph {
    Node nodes[MAX_PAGES];
    int num_nodes;
} Graph;

// 웹 크롤러 함수
void crawl(Graph* graph, Node* node) {
    printf("Crawling %s\n", node->url);
    // 웹페이지에서 하이퍼링크 추출
    // 추출한 링크를 그래프에 추가하고 엣지 연결
}

// 페이지 랭킹 계산 함수
void calculate_page_rank(Graph* graph) {
    int i, j;
    float initial_rank = 1.0 / graph->num_nodes;

    // 초기 페이지 랭크 설정
    for (i = 0; i < graph->num_nodes; i++) {
        graph->nodes[i].page_rank = initial_rank;
    }

    bool has_converged = false;
    float diff;
    float new_rank[MAX_PAGES];

    // 페이지 랭킹 반복 계산
    while (!has_converged) {
        for (i = 0; i < graph->num_nodes; i++) {
            new_rank[i] = (1 - DAMPING_FACTOR) / graph->num_nodes;

            for (j = 0; j < graph->nodes[i].num_edges; j++) {
                Node* neighbor = graph->nodes[i].edges[j];
                new_rank[i] += DAMPING_FACTOR * neighbor->page_rank / neighbor->num_edges;
            }
        }

        // 수렴 여부 확인
        diff = 0;
        for (i = 0; i < graph->num_nodes; i++) {
            diff += fabs(new_rank[i] - graph->nodes[i].page_rank);
        }

        if (diff < EPSILON)
            has_converged = true;

        // 새로운 페이지 랭크 적용
        for (i = 0; i < graph->num_nodes; i++) {
            graph->nodes[i].page_rank = new_rank[i];
        }
    }
}

int main() {
    // 초기 그래프 생성
    Graph graph;
    graph.num_nodes = 0;

    // 그래프에 초기 노드 추가
    strcpy(graph.nodes[graph.num_nodes].url, "http://example.com/page1");
    graph.nodes[graph.num_nodes++].num_edges = 0;

    // 추가 페이지 추가 및 연결
    strcpy(graph.nodes[graph.num_nodes].url, "http://example.com/page2");
    graph.nodes[graph.num_nodes].edges[0] = &graph.nodes[0];
    graph.nodes[graph.num_nodes].num_edges = 1;
    graph.num_nodes++;

    strcpy(graph.nodes[graph.num_nodes].url, "http://example.com/page3");
    graph.nodes[graph.num_nodes].edges[0] = &graph.nodes[0];
    graph.nodes[graph.num_nodes].edges[1] = &graph.nodes[1];
    graph.nodes[graph.num_nodes].num_edges = 2;
    graph.num_nodes++;

    strcpy(graph.nodes[graph.num_nodes].url, "http://example.com/page4");
    graph.nodes[graph.num_nodes].edges[0] = &graph.nodes[0];
    graph.nodes[graph.num_nodes].edges[1] = &graph.nodes[1];
    graph.nodes[graph.num_nodes].edges[2] = &graph.nodes[2];
    graph.nodes[graph.num_nodes].num_edges = 3;
    graph.num_nodes++;

    strcpy(graph.nodes[graph.num_nodes].url, "http://example.com/page5");
    graph.nodes[graph.num_nodes].edges[0] = &graph.nodes[0];
    graph.nodes[graph.num_nodes].edges[1] = &graph.nodes[1];
    graph.nodes[graph.num_nodes].edges[2] = &graph.nodes[2];
    graph.nodes[graph.num_nodes].edges[3] = &graph.nodes[3];
    graph.nodes[graph.num_nodes].num_edges = 4;
    graph.num_nodes++;

    // 그래프 크롤링
    for (int i = 0; i < graph.num_nodes; i++) {
        crawl(&graph, &graph.nodes[i]);
    }

    // 페이지 랭킹 계산
    calculate_page_rank(&graph);

    // 결과 출력
    for (int i = 0; i < graph.num_nodes; i++) {
        printf("Page: %s, Rank: %f\n", graph.nodes[i].url, graph.nodes[i].page_rank);
    }

    return 0;
}
