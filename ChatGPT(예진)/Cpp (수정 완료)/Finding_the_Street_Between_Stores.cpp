#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

// Store class definition
class Store {
public:
    string name;
    float x;
    float y;

    Store(string n, float a, float b) : name(n), x(a), y(b) {}
};

// Calculate the Euclidean distance between two points
float euclidean_distance(const Store& point1, const Store& point2) {
    return sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2));
}

// Find the closest and farthest two stores
void find_closest_and_farthest(const vector<Store>& stores, vector<string>& closest_stores, float& min_distance, vector<string>& farthest_stores, float& max_distance) {
    min_distance = numeric_limits<float>::infinity();
    max_distance = -numeric_limits<float>::infinity();

    for (size_t i = 0; i < stores.size(); i++) {
        for (size_t j = i + 1; j < stores.size(); j++) {
            float distance = euclidean_distance(stores[i], stores[j]);
            if (distance < min_distance) {
                min_distance = distance;
                closest_stores[0] = stores[i].name;
                closest_stores[1] = stores[j].name;
            }
            if (distance > max_distance) {
                max_distance = distance;
                farthest_stores[0] = stores[i].name;
                farthest_stores[1] = stores[j].name;
            }
        }
    }
}

// Main function
int main() {
    vector<Store> stores;
    vector<string> closest_stores(2);
    vector<string> farthest_stores(2);
    float min_distance, max_distance;

    cout << "Enter the locations of three stores." << endl;
    for (int i = 0; i < 3; i++) {
        string store_name;
        float x, y;
        cout << "Enter the store name: ";
        cin >> store_name;
        cout << "Enter the x coordinate of the store: ";
        cin >> x;
        cout << "Enter the y coordinate of the store: ";
        cin >> y;
        stores.push_back(Store(store_name, x, y));
    }

    find_closest_and_farthest(stores, closest_stores, min_distance, farthest_stores, max_distance);
    cout << "Closest two stores: " << closest_stores[0] << ", " << closest_stores[1] << " Distance: " << min_distance << endl;
    cout << "Farthest two stores: " << farthest_stores[0] << ", " << farthest_stores[1] << " Distance: " << max_distance << endl;

    return 0;
}