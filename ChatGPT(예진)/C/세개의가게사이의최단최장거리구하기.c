#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

// 가게 구조체 정의
struct Store {
    char name[50];
    float x;
    float y;
};

// 가게 위치 입력 받기
void input_store_locations(struct Store stores[]) {
    for (int i = 0; i < 3; i++) {
        printf("가게 이름을 입력하세요: ");
        scanf("%s", stores[i].name);
        printf("가게의 x 좌표를 입력하세요: ");
        scanf("%f", &stores[i].x);
        printf("가게의 y 좌표를 입력하세요: ");
        scanf("%f", &stores[i].y);
    }
}

// 두 점 사이의 유클리드 거리 계산
float euclidean_distance(struct Store point1, struct Store point2) {
    return sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2));
}

// 가장 가까운 두 개의 가게와 가장 먼 두 개의 가게 찾기
void find_closest_and_farthest(struct Store stores[], char closest_stores[][50], float* min_distance, char farthest_stores[][50], float* max_distance) {
    *min_distance = INFINITY;
    *max_distance = -INFINITY;

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            float distance = euclidean_distance(stores[i], stores[j]);
            if (distance < *min_distance) {
                *min_distance = distance;
                sprintf(closest_stores[0], "%s", stores[i].name);
                sprintf(closest_stores[1], "%s", stores[j].name);
            }
            if (distance > *max_distance) {
                *max_distance = distance;
                sprintf(farthest_stores[0], "%s", stores[i].name);
                sprintf(farthest_stores[1], "%s", stores[j].name);
            }
        }
    }
}

// 메인 함수
int main() {
    struct Store stores[3];
    char closest_stores[2][50];
    char farthest_stores[2][50];
    float min_distance, max_distance;

    printf("세 개의 가게 위치를 입력하세요.\n");
    input_store_locations(stores);
    find_closest_and_farthest(stores, closest_stores, &min_distance, farthest_stores, &max_distance);
    printf("가장 가까운 두 개의 가게: %s, %s 거리: %.2f\n", closest_stores[0], closest_stores[1], min_distance);
    printf("가장 먼 두 개의 가게: %s, %s 거리: %.2f\n", farthest_stores[0], farthest_stores[1], max_distance);

    return 0;
}
