# 휴리스틱 탐색 알고리즘을 구현해 주세요.
# 휴리스틱 함수는 다음과 같습니다.
# h(n) = |x1 - x2| + |y1 - y2|
# 여기서 x1, y1은 현재 노드의 좌표이고, x2, y2는 목표 노드의 좌표입니다.
# 이 휴리스틱 함수를 이용하여 A* 알고리즘을 구현해 주세요.
# A* 알고리즘은 다음과 같습니다.
# 1. 시작 노드를 open 리스트에 넣습니다.
# 2. open 리스트가 비어있지 않은 동안 다음을 반복합니다.
#     1. open 리스트에서 가장 작은 f(n)을 가진 노드 n을 꺼냅니다.
#     2. n이 목표 노드라면 경로를 출력하고 종료합니다.
#     3. n을 closed 리스트에 넣습니다.
#     4. n의 이웃 노드들에 대해 다음을 반복합니다.
#         1. 이웃 노드가 closed 리스트에 있거나 벽이라면 무시합니다.
#         2. 이웃 노드가 open 리스트에 없다면 open 리스트에 넣습니다.
#         3. 이웃 노드가 open 리스트에 있다면 현재 경로가 더 나은 경로라면 이웃 노드의 f(n)을 업데이트합니다.
# 3. open 리스트가 비어있고 목표 노드를 찾지 못했다면 실패를 출력합니다.
# 여기서 f(n)은 다음과 같습니다.
# f(n) = g(n) + h(n)
# 여기서 g(n)은 시작 노드부터 n까지의 실제 비용입니다.
# g(n)은 다음과 같이 계산합니다.
# g(n) = g(parent) + cost(parent, n)
# 여기서 parent는 n의 부모 노드이고, cost(parent, n)은 parent에서 n까지의 비용입니다.
# cost(parent, n)은 항상 1이라고 가정합니다.
# 또한, 이웃 노드들은 상하좌우로만 움직일 수 있습니다.
# 시작 노드와 끝 노드는 사용자가 입력합니다.
# 사용자가 벽을 추가할 수 있습니다.
# 벽은 사용자가 입력한 좌표를 포함하는 칸입니다.
# 경로를 찾았다면 출력해 주세요.
# 조건에 맞춰 개발해 주세요.

def h(node, goal):
    return abs(node[0] - goal[0]) + abs(node[1] - goal[1])

def a_star(start, goal, walls):
    open_list = [start]
    closed_list = []
    g = {start: 0}
    f = {start: h(start, goal)}
    parent = {start: None}

    while open_list:
        current = min(open_list, key=lambda x: f[x])
        open_list.remove(current)

        if current == goal:
            path = []
            while current:
                path.append(current)
                current = parent[current]
            path.reverse()
            return path

        closed_list.append(current)

        for i, j in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
            neighbor = (current[0] + i, current[1] + j)
            if neighbor in walls or neighbor in closed_list:
                continue

            if neighbor not in open_list:
                open_list.append(neighbor)

            if g[current] + 1 < g.get(neighbor, float('inf')):
                g[neighbor] = g[current] + 1
                f[neighbor] = g[neighbor] + h(neighbor, goal)
                parent[neighbor] = current

    return 'Fail'

# 입력 처리
print("Enter the start node:")
start = tuple(map(int, input().split()))
print("Enter the end node:")
goal = tuple(map(int, input().split()))
print("Enter the number of walls:")
n = int(input())
walls = set()
for _ in range(n):
    print("Enter the wall node:")
    wall = tuple(map(int, input().split()))
    walls.add(wall)

# 경로 탐색 및 출력
path = a_star(start, goal, walls)
if path == 'Fail':
    print("No path found")
else:
    print("Path found:", path)
