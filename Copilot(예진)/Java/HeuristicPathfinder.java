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
// java로 구현해 주세요.
// 경로를 찾았다면 출력해 주세요.

import java.util.*;

class Node {
    int x;
    int y;
    int f;
    int g;
    int h;
    Node parent;

    public Node(int x, int y) {
        this.x = x;
        this.y = y;
        this.f = 0;
        this.g = 0;
        this.h = 0;
        this.parent = null;
    }
}

public class HeuristicPathfinder {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter the size of the grid: ");
        int n = scanner.nextInt();
        int[][] grid = new int[n][n];
        System.out.print("Enter the start node: ");
        int startX = scanner.nextInt();
        int startY = scanner.nextInt();
        System.out.print("Enter the end node: ");
        int endX = scanner.nextInt();
        int endY = scanner.nextInt();
        System.out.print("Enter the number of walls: ");
        int walls = scanner.nextInt();
        for (int i = 0; i < walls; i++) {
            System.out.print("Enter the wall node: ");
            int wallX = scanner.nextInt();
            int wallY = scanner.nextInt();
            grid[wallX][wallY] = 1;
        }
        scanner.close();
        Node start = new Node(startX, startY);
        Node end = new Node(endX, endY);
        List<Node> open = new ArrayList<>();
        List<Node> closed = new ArrayList<>();
        open.add(start);
        while (!open.isEmpty()) {
            Node current = open.get(0);
            for (int i = 1; i < open.size(); i++) {
                if (open.get(i).f < current.f) {
                    current = open.get(i);
                }
            }
            open.remove(current);
            closed.add(current);
            if (current.x == end.x && current.y == end.y) {
                List<Node> path = new ArrayList<>();
                while (current != null) {
                    path.add(current);
                    current = current.parent;
                }
                Collections.reverse(path);
                for (Node node : path) {
                    System.out.println(node.x + " " + node.y);
                }
                return;
            }
            int[] dx = {0, 0, -1, 1};
            int[] dy = {-1, 1, 0, 0};
            for (int i = 0; i < 4; i++) {
                int x = current.x + dx[i];
                int y = current.y + dy[i];
                if (x < 0 || x >= n || y < 0 || y >= n || grid[x][y] == 1) {
                    continue;
                }
                Node neighbor = new Node(x, y
                );

                if (closed.contains(neighbor)) {
                    continue;
                }
                int g = current.g + 1;
                boolean newPath = false;
                if (open.contains(neighbor)) {
                    if (g < neighbor.g) {
                        neighbor.g = g;
                        newPath = true;
                    }
                } else {
                    neighbor.g = g;
                    newPath = true;
                    open.add(neighbor);
                }
                if (newPath) {
                    neighbor.h = Math.abs(neighbor.x - end.x) + Math.abs(neighbor.y - end.y);
                    neighbor.f = neighbor.g + neighbor.h;
                    neighbor.parent = current;
                }
            }
        }
        System.out.println("Failed");
    }
}

