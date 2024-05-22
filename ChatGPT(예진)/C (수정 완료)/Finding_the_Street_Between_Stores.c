#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

// Define the Store structure
struct Store {
    char name[50];
    float x;
    float y;
};

// Function to input store locations
void input_store_locations(struct Store stores[]) {
    for (int i = 0; i < 3; i++) {
        printf("Enter the store name: ");
        scanf("%s", stores[i].name);
        printf("Enter the x-coordinate of the store: ");
        scanf("%f", &stores[i].x);
        printf("Enter the y-coordinate of the store: ");
        scanf("%f", &stores[i].y);
    }
}

// Function to calculate the Euclidean distance between two points
float euclidean_distance(struct Store point1, struct Store point2) {
    return sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2));
}

// Function to find the closest and farthest two stores
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

// Main function
int main() {
    struct Store stores[3];
    char closest_stores[2][50];
    char farthest_stores[2][50];
    float min_distance, max_distance;

    printf("Enter the locations of three stores.\n");
    input_store_locations(stores);
    find_closest_and_farthest(stores, closest_stores, &min_distance, farthest_stores, &max_distance);
    printf("The closest two stores are: %s, %s Distance: %.2f\n", closest_stores[0], closest_stores[1], min_distance);
    printf("The farthest two stores are: %s, %s Distance: %.2f\n", farthest_stores[0], farthest_stores[1], max_distance);

    return 0;
}
