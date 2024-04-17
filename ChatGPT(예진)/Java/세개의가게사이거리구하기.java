import java.util.Scanner;

public class 세개의가게사이거리구하기 {
    
    // 두 점 사이의 유클리드 거리 계산
    public static double euclideanDistance(double[] point1, double[] point2) {
        return Math.sqrt(Math.pow((point1[0] - point2[0]), 2) + Math.pow((point1[1] - point2[1]), 2));
    }

    // 가장 가까운 두 개의 가게와 가장 먼 두 개의 가게 찾기
    public static Object[] findClosestAndFarthest(double[][] stores) {
        double minDistance = Double.POSITIVE_INFINITY;
        double maxDistance = Double.NEGATIVE_INFINITY;
        String[] closestStores = new String[2];
        String[] farthestStores = new String[2];

        for (int i = 0; i < stores.length; i++) {
            for (int j = i + 1; j < stores.length; j++) {
                double distance = euclideanDistance(stores[i], stores[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestStores[0] = String.valueOf(stores[i][0]);
                    closestStores[1] = String.valueOf(stores[j][0]);
                }
                if (distance > maxDistance) {
                    maxDistance = distance;
                    farthestStores[0] = String.valueOf(stores[i][0]);
                    farthestStores[1] = String.valueOf(stores[j][0]);
                }
            }
        }

        Object[] result = { closestStores, minDistance, farthestStores, maxDistance };
        return result;
    }

    // 메인 메서드
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("세 개의 가게 위치를 입력하세요.");
        double[][] stores = new double[3][3];
        for (int i = 0; i < 3; i++) {
            System.out.print("가게 이름을 입력하세요: ");
            String storeName = scanner.next();
            System.out.print("가게의 x 좌표를 입력하세요: ");
            double x = scanner.nextDouble();
            System.out.print("가게의 y 좌표를 입력하세요: ");
            double y = scanner.nextDouble();
            stores[i][0] = x;
            stores[i][1] = y;
        }

        Object[] distances = findClosestAndFarthest(stores);
        String[] closestStores = (String[]) distances[0];
        double minDistance = (double) distances[1];
        String[] farthestStores = (String[]) distances[2];
        double maxDistance = (double) distances[3];

        System.out.println("가장 가까운 두 개의 가게: " + closestStores[0] + ", " + closestStores[1] + " 거리: " + minDistance);
        System.out.println("가장 먼 두 개의 가게: " + farthestStores[0] + ", " + farthestStores[1] + " 거리: " + maxDistance);
        
        scanner.close();
    }
}
