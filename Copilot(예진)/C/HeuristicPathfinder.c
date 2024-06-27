// �޸���ƽ Ž�� �˰����� ������ �ּ���.
// �޸���ƽ �Լ��� ������ �����ϴ�.
// h(n) = |x1 - x2| + |y1 - y2|
// ���⼭ x1, y1�� ���� ����� ��ǥ�̰�, x2, y2�� ��ǥ ����� ��ǥ�Դϴ�.
// �� �޸���ƽ �Լ��� �̿��Ͽ� A* �˰����� ������ �ּ���.
// A* �˰����� ������ �����ϴ�.
// 1. ���� ��带 open ����Ʈ�� �ֽ��ϴ�.
// 2. open ����Ʈ�� ������� ���� ���� ������ �ݺ��մϴ�.
//     1. open ����Ʈ���� ���� ���� f(n)�� ���� ��� n�� �����ϴ�.
//     2. n�� ��ǥ ����� ��θ� ����ϰ� �����մϴ�.
//     3. n�� closed ����Ʈ�� �ֽ��ϴ�.
//     4. n�� �̿� ���鿡 ���� ������ �ݺ��մϴ�.
//         1. �̿� ��尡 closed ����Ʈ�� �ְų� ���̶�� �����մϴ�.
//         2. �̿� ��尡 open ����Ʈ�� ���ٸ� open ����Ʈ�� �ֽ��ϴ�.
//         3. �̿� ��尡 open ����Ʈ�� �ִٸ� ���� ��ΰ� �� ���� ��ζ�� �̿� ����� f(n)�� ������Ʈ�մϴ�.
// 3. open ����Ʈ�� ����ְ� ��ǥ ��带 ã�� ���ߴٸ� ���и� ����մϴ�.
// ���⼭ f(n)�� ������ �����ϴ�.
// f(n) = g(n) + h(n)
// ���⼭ g(n)�� ���� ������ n������ ���� ����Դϴ�.
// g(n)�� ������ ���� ����մϴ�.
// g(n) = g(parent) + cost(parent, n)
// ���⼭ parent�� n�� �θ� ����̰�, cost(parent, n)�� parent���� n������ ����Դϴ�.
// cost(parent, n)�� �׻� 1�̶�� �����մϴ�.
// ����, �̿� ������ �����¿�θ� ������ �� �ֽ��ϴ�.
// ���� ���� �� ���� ����ڰ� �Է��մϴ�.
// ����ڰ� ���� �߰��� �� �ֽ��ϴ�.
// ���� ����ڰ� �Է��� ��ǥ�� �����ϴ� ĭ�Դϴ�.
// c���� ������ �ּ���.
// ��θ� ã�Ҵٸ� ����� �ּ���.

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

