import math

# Define the Store class
class Store:
    def __init__(self):
        self.name = ""
        self.x = 0.0
        self.y = 0.0

# Function to input store locations
def input_store_locations(stores):
    for i in range(3):
        stores[i].name = input("Enter the store name: ")
        stores[i].x = float(input("Enter the x-coordinate of the store: "))
        stores[i].y = float(input("Enter the y-coordinate of the store: "))

# Function to calculate the Euclidean distance between two points
def euclidean_distance(point1, point2):
    return math.sqrt((point1.x - point2.x) ** 2 + (point1.y - point2.y) ** 2)

# Function to find the closest and farthest two stores
def find_closest_and_farthest(stores):
    min_distance = float('inf')
    max_distance = float('-inf')
    closest_stores = ["", ""]
    farthest_stores = ["", ""]

    for i in range(3):
        for j in range(i + 1, 3):
            distance = euclidean_distance(stores[i], stores[j])
            if distance < min_distance:
                min_distance = distance
                closest_stores[0] = stores[i].name
                closest_stores[1] = stores[j].name
            if distance > max_distance:
                max_distance = distance
                farthest_stores[0] = stores[i].name
                farthest_stores[1] = stores[j].name

    return closest_stores, min_distance, farthest_stores, max_distance

# Main function
def main():
    stores = [Store() for _ in range(3)]

    print("Enter the locations of three stores.")
    input_store_locations(stores)
    closest_stores, min_distance, farthest_stores, max_distance = find_closest_and_farthest(stores)
    print(f"The closest two stores are: {closest_stores[0]}, {closest_stores[1]} Distance: {min_distance:.2f}")
    print(f"The farthest two stores are: {farthest_stores[0]}, {farthest_stores[1]} Distance: {max_distance:.2f}")

if __name__ == "__main__":
    main()
