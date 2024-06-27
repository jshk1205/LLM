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
// c++�� ������ �ּ���.
// ��θ� ã�Ҵٸ� ����� �ּ���.

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;

#define MAX 100

struct Node
{
    int x;
    int y;
    int f;
    int g;
    int h;
    Node* parent;
};

struct List
{
    Node* array[MAX];
    int size;
};

List open;
List closed;
int map[MAX][MAX];
int n, m;
int start_x, start_y, end_x, end_y;

void init()
{
    open.size = 0;
    closed.size = 0;
    for (int i = 0; i < MAX; i++)
    {
        open.array[i] = NULL;
        closed.array[i] = NULL;
    }
}

int heuristic(Node* current)
{
    return abs(current->x - end_x) + abs(current->y - end_y);
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

void print_path(Node* node)
{
    if (node == NULL)
    {
        return;
    }

    print_path(node->parent);
    cout << "(" << node->x << ", " << node->y << ") ";
}

int main()
{
    cout << "Enter the size of the map: ";
    cin >> n >> m;

    cout << "Enter the start node: ";
    cin >> start_x >> start_y;

    cout << "Enter the end node: ";
    cin >> end_x >> end_y;

    cout << "Enter the number of walls: ";
    int num_walls;
    cin >> num_walls;

    cout << "Enter the coordinates of the walls: ";
    for (int i = 0; i < num_walls; i++)
    {
        int x, y;
        cin >> x >> y;
        map[x][y] = 1;
    }

    Node* start = new Node;
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
            cout << "Path found: ";
            print_path(current);
            cout << endl;
            return 0;
        }

        push(&closed, current);

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }

                int new_x = current->x + i;
                int new_y = current->y + j;

                if (!is_valid(new_x, new_y) || is_wall(new_x, new_y))
                {
                    continue;
                }

                Node* neighbor = new Node;
                neighbor->x = new_x;
                neighbor->y = new_y;
                neighbor->g = current->g + 1;
                neighbor->h = heuristic(neighbor);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;

                bool in_open = false;
                for (int k = 0; k < open.size; k++)
                {
                    if (open.array[k]->x == neighbor->x && open.array[k]->y == neighbor->y)
                    {
                        in_open = true;
                        break;
                    }
                }

                if (!in_open)
                {
                    bool in_closed = false;
                    for (int k = 0; k < closed.size; k++)
                    {
                        if (closed.array[k]->x == neighbor->x && closed.array[k]->y == neighbor->y)
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
    }

    cout << "Path not found" << endl;

    return 0;
}

