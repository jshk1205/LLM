import java.util.Scanner;
import java.lang.Math;

class Store {
    String name;
    float x;
    float y;
}

public class Finding_the_Street_Between_Stores {

    // Function to input store locations
    public static void inputStoreLocations(Store[] stores) {
        Scanner scanner = new Scanner(System.in);
        for (int i = 0; i < 3; i++) {
            stores[i] = new Store();
            System.out.print("Enter the store name: ");
            stores[i].name = scanner.nextLine();
            System.out.print("Enter the x-coordinate of the store: ");
            stores[i].x = scanner.nextFloat();
            System.out.print("Enter the y-coordinate of the store: ");
            stores[i].y = scanner.nextFloat();
            scanner.nextLine(); // consume newline
        }
    }

    // Function to calculate the Euclidean distance between two points
    public static float euclideanDistance(Store point1, Store point2) {
        return (float) Math.sqrt(Math.pow((point1.x - point2.x), 2) + Math.pow((point1.y - point2.y), 2));
    }

    // Function to find the closest and farthest two stores
    public static void findClosestAndFarthest(Store[] stores, String[] closestStores, float[] minDistance, String[] farthestStores, float[] maxDistance) {
        minDistance[0] = Float.POSITIVE_INFINITY;
        maxDistance[0] = Float.NEGATIVE_INFINITY;

        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                float distance = euclideanDistance(stores[i], stores[j]);
                if (distance < minDistance[0]) {
                    minDistance[0] = distance;
                    closestStores[0] = stores[i].name;
                    closestStores[1] = stores[j].name;
                }
                if (distance > maxDistance[0]) {
                    maxDistance[0] = distance;
                    farthestStores[0] = stores[i].name;
                    farthestStores[1] = stores[j].name;
                }
            }
        }
    }

    public static void main(String[] args) {
        Store[] stores = new Store[3];
        String[] closestStores = new String[2];
        String[] farthestStores = new String[2];
        float[] minDistance = new float[1];
        float[] maxDistance = new float[1];

        System.out.println("Enter the locations of three stores.");
        inputStoreLocations(stores);
        findClosestAndFarthest(stores, closestStores, minDistance, farthestStores, maxDistance);
        System.out.printf("The closest two stores are: %s, %s Distance: %.2f\n", closestStores[0], closestStores[1], minDistance[0]);
        System.out.printf("The farthest two stores are: %s, %s Distance: %.2f\n", farthestStores[0], farthestStores[1], maxDistance[0]);
    }
}
