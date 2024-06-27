// 휴리스틱 탐색 알고리즘을 구현해 주세요.
// 휴리스틱 함수는 다음과 같습니다.
// h(n) = |x1 - x2| + |y1 - y2|
// 여기서 x1, y1은 현재 노드의 좌표이고, x2, y2는 목표 노드의 좌표입니다.
// 이 휴리스틱 함수를 이용하여 A* 알고리즘을 구현해 주세요.
// A* 알고리즘은 다음과 같습니다.
// 1. 시작 노드를 open 리스트에 넣습니다.
// 2. open 리스트가 비어있지 않은 동안 다음을 반복합니다.
//     1. open 리스트에서 가장 작은 f(n)을 가진 노드 n을 꺼냅니다.
//     2. n이 목표 노드라면 경로를 출력하고 종료합니다.
//     3. n을 closed 리스트에 넣습니다.
//     4. n의 이웃 노드들에 대해 다음을 반복합니다.
//         1. 이웃 노드가 closed 리스트에 있거나 벽이라면 무시합니다.
//         2. 이웃 노드가 open 리스트에 없다면 open 리스트에 넣습니다.
//         3. 이웃 노드가 open 리스트에 있다면 현재 경로가 더 나은 경로라면 이웃 노드의 f(n)을 업데이트합니다.
// 3. open 리스트가 비어있고 목표 노드를 찾지 못했다면 실패를 출력합니다.
// 여기서 f(n)은 다음과 같습니다.
// f(n) = g(n) + h(n)
// 여기서 g(n)은 시작 노드부터 n까지의 실제 비용입니다.
// g(n)은 다음과 같이 계산합니다.
// g(n) = g(parent) + cost(parent, n)
// 여기서 parent는 n의 부모 노드이고, cost(parent, n)은 parent에서 n까지의 비용입니다.
// cost(parent, n)은 항상 1이라고 가정합니다.
// 또한, 이웃 노드들은 상하좌우로만 움직일 수 있습니다.
// 시작 노드와 끝 노드는 사용자가 입력합니다.
// 사용자가 벽을 추가할 수 있습니다.
// 벽은 사용자가 입력한 좌표를 포함하는 칸입니다.
// c언어로 구현해 주세요.
// 경로를 찾았다면 출력해 주세요.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100

typedef struct Node
{
    int x;
    int y;
    int f;
    int g;
    int h;
    struct Node* parent;
} Node;

typedef struct List
{
    Node* array[MAX];
    int size;
} List;

List open;
List closed;
int map[MAX][MAX];
int n, m;
int start_x, start_y, end_x, end_y;

void init()
{
    open.size = 0;
    closed.size = 0;
}

void push(List* list, Node* node)
{
    list->array[list->size++] = node;
}

Node* pop(List* list)
{
    int min = 0;
    for (int i = 1; i < list->size; i++)
    {
        if (list->array[i]->f < list->array[min]->f)
        {
            min = i;
        }
    }

    Node* node = list->array[min];
    for (int i = min; i < list->size - 1; i++)
    {
        list->array[i] = list->array[i + 1];
    }
    list->size--;

    return node;
}

bool is_valid(int x, int y)
{
    return x >= 0 && x < n && y >= 0 && y < m;
}

bool is_wall(int x, int y)
{
    return map[x][y] == 1;
}

int heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

void print_path(Node* node)
{
    if (node == NULL)
    {
        return;
    }

    print_path(node->parent);
    printf("(%d, %d) ", node->x, node->y);
}

void a_star()
{
    Node* start = (Node*)malloc(sizeof(Node));
    start->x = start_x;
    start->y = start_y;
    start->f = 0;
    start->g = 0;
    start->h = 0;
    start->parent = NULL;

    push(&open, start);

    while (open.size > 0)
    {
        Node* current = pop(&open);

        if (current->x == end_x && current->y == end_y)
        {
            print_path(current);
            return;
        }

        push(&closed, current);

        int dx[] = { 0, 0, -1, 1 };
        int dy[] = { -1, 1, 0, 0 };

        for (int i = 0; i < 4; i++)
        {
            int x = current->x + dx[i];
            int y = current->y + dy[i];

            if (!is_valid(x, y) || is_wall(x, y))
            {
                continue;
            }

            Node* neighbor = (Node*)malloc(sizeof(Node));
            neighbor->x = x;
            neighbor->y = y;
            neighbor->g = current->g + 1;
            neighbor->h = heuristic(x, y, end_x, end_y);
            neighbor->f = neighbor->g + neighbor->h;
            neighbor->parent = current;

            bool in_open = false;
            for (int j = 0; j < open.size; j++)
            {
                if (open.array[j]->x == neighbor->x && open.array[j]->y == neighbor->y)
                {
                    in_open = true;
                    break;
                }
            }

            if (!in_open)
            {
                bool in_closed = false;
                for (int j = 0; j < closed.size; j++)
                {
                    if (closed.array[j]->x == neighbor->x && closed.array[j]->y == neighbor->y)
                    {
                        in_closed = true;
                        break;
                    }
                }

                if (!in_closed)
                {
                    push(&open, neighbor);
                }
            }
        }
    }

    printf("Fail\n");
}

int main()
{
    printf("Enter the size of the map: ");
    scanf("%d %d", &n, &m);

    printf("Enter the map:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &map[i][j]);
        }
    }

    printf("Enter the start node: ");
    scanf("%d %d", &start_x, &start_y);

    printf("Enter the end node: ");
    scanf("%d %d", &end_x, &end_y);

    a_star();

    return 0;
}

