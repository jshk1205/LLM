import java.util.ArrayList;
import java.util.List;

// 아무리 기다려도 rank 결과가 뜨지 않음
// gpt한테 코드를 수정해달라고 했는데도 계속해서 rank 결과는 안 뜸

class Node {
    String url;
    List<Node> edges;
    int numEdges;
    float pageRank;

    Node(String url) {
        this.url = url;
        this.edges = new ArrayList<>();
        this.pageRank = 0.0f;
    }
}

class Graph {
    List<Node> nodes;

    Graph() {
        this.nodes = new ArrayList<>();
    }

    void crawl(Node node) {
        System.out.println("Crawling " + node.url);
        // Implement crawling logic here
    }

    void calculatePageRank() {
        int numNodes = nodes.size();
        float initialRank = 1.0f / numNodes;

        // Initialize page ranks
        for (Node node : nodes) {
            node.pageRank = initialRank;
        }

        boolean hasConverged = false;
        float epsilon = 0.0001f;
        float dampingFactor = 0.85f;
        float[] newRank = new float[numNodes];

        while (!hasConverged) {
            for (int i = 0; i < numNodes; i++) {
                newRank[i] = (1 - dampingFactor) / numNodes;

                for (Node neighbor : nodes.get(i).edges) {
                    newRank[i] += dampingFactor * neighbor.pageRank / neighbor.numEdges;
                }
            }

            // Check convergence
            float diff = 0;
            for (int i = 0; i < numNodes; i++) {
                diff += Math.abs(newRank[i] - nodes.get(i).pageRank);
            }

            if (diff < epsilon) {
                hasConverged = true;
            }

            // Update page ranks
            for (int i = 0; i < numNodes; i++) {
                nodes.get(i).pageRank = newRank[i];
            }
        }
    }
}

public class WebRanker {
    public static void main(String[] args) {
        Graph graph = new Graph();

        // Add nodes to the graph
        Node naverNode = new Node("https://www.naver.com/");
        Node daumNode = new Node("https://www.daum.net/");
        Node kakaoMapNode = new Node("https://map.kakao.com/?referrer=daumtop");
        Node naverBlogNode = new Node("https://section.blog.naver.com/BlogHome.naver");
        Node tistoryNode = new Node("https://www.tistory.com/");

        naverNode.edges.add(daumNode);
        daumNode.edges.add(naverNode);
        kakaoMapNode.edges.add(naverNode);
        kakaoMapNode.edges.add(daumNode);
        naverBlogNode.edges.add(naverNode);
        naverBlogNode.edges.add(daumNode);
        naverBlogNode.edges.add(kakaoMapNode);
        tistoryNode.edges.add(naverNode);
        tistoryNode.edges.add(daumNode);
        tistoryNode.edges.add(kakaoMapNode);
        tistoryNode.edges.add(naverBlogNode);

        graph.nodes.add(naverNode);
        graph.nodes.add(daumNode);
        graph.nodes.add(kakaoMapNode);
        graph.nodes.add(naverBlogNode);
        graph.nodes.add(tistoryNode);

        // Crawl the nodes
        for (Node node : graph.nodes) {
            graph.crawl(node);
        }

        // Calculate page ranks
        graph.calculatePageRank();

        // Print page ranks
        for (Node node : graph.nodes) {
            System.out.println("Page: " + node.url + ", Rank: " + node.pageRank);
        }
    }
}
