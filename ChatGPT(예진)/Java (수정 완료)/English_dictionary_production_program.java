import java.util.*;

class TrieNode {
    TrieNode[] children = new TrieNode[26]; // 26 lowercase letters
    boolean endOfWord;
    String definition;
    List<String> synonyms;
    List<String> antonyms;
    List<String> examples;
    int frequency;
    String pronunciation;

    TrieNode() {
        endOfWord = false;
        definition = null;
        synonyms = new ArrayList<>();
        antonyms = new ArrayList<>();
        examples = new ArrayList<>();
        frequency = 0;
        pronunciation = null;
    }
}

class Dictionary {
    TrieNode root;
    String logFile;

    Dictionary() {
        root = new TrieNode();
        logFile = "dictionary_log.txt"; // In Java, you would handle this differently.
    }
}

public class English_dictionary_production_program {
    
    public static void addEntry(Dictionary dict, String word, String definition, List<String> synonyms, List<String> antonyms, List<String> examples, String pronunciation) {
        TrieNode node = dict.root;
        for (char c : word.toCharArray()) {
            int index = c - 'a';
            if (node.children[index] == null) {
                node.children[index] = new TrieNode();
            }
            node = node.children[index];
        }
        node.endOfWord = true;
        node.definition = definition;
        node.synonyms = synonyms;
        node.antonyms = antonyms;
        node.examples = examples;
        node.pronunciation = pronunciation;
        node.frequency++;
    }

    public static void removeEntry(Dictionary dict, String word) {
        TrieNode node = dict.root;
        for (char c : word.toCharArray()) {
            int index = c - 'a';
            if (node.children[index] == null) {
                System.out.printf("'%s' does not exist.%n", word);
                return;
            }
            node = node.children[index];
        }
        if (!node.endOfWord) {
            System.out.printf("'%s' does not exist.%n", word);
            return;
        }
        node.endOfWord = false;
        node.frequency--;
        System.out.printf("'%s' has been removed.%n", word);
    }

    public static void searchWord(Dictionary dict, String word) {
        TrieNode node = dict.root;
        for (char c : word.toCharArray()) {
            int index = c - 'a';
            if (node.children[index] == null) {
                System.out.printf("'%s' not found.%n", word);
                return;
            }
            node = node.children[index];
        }
        if (!node.endOfWord) {
            System.out.printf("'%s' not found.%n", word);
            return;
        }
        System.out.printf("'%s' definition: %s%n", word, node.definition);
        System.out.printf("Pronunciation: %s%n", node.pronunciation);
        System.out.printf("Synonyms: %s%n", String.join(", ", node.synonyms));
        System.out.printf("Antonyms: %s%n", String.join(", ", node.antonyms));
        System.out.printf("Examples:%n");
        for (String example : node.examples) {
            System.out.printf("- %s%n", example);
        }
        System.out.printf("Frequency: %d%n", node.frequency);
    }

    public static void listWordsFromNode(TrieNode node, String prefix) {
        if (node.endOfWord) {
            System.out.println(prefix);
        }
        for (int i = 0; i < 26; i++) {
            if (node.children[i] != null) {
                listWordsFromNode(node.children[i], prefix + (char) ('a' + i));
            }
        }
    }

    public static void listAllWords(Dictionary dict, String prefix) {
        TrieNode node = dict.root;
        for (char c : prefix.toCharArray()) {
            int index = c - 'a';
            if (node.children[index] == null) {
                System.out.println("No words found with the given prefix.");
                return;
            }
            node = node.children[index];
        }
        listWordsFromNode(node, prefix);
    }

    public static void mainMenu(Dictionary dict) {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("\nDictionary Program");
            System.out.println("1. Add word");
            System.out.println("2. Remove word");
            System.out.println("3. Search word");
            System.out.println("4. List all words");
            System.out.println("5. Exit");
            System.out.print("Enter your choice: ");

            int choice = scanner.nextInt();
            scanner.nextLine(); // Consume the newline

            switch (choice) {
                case 1: {
                    System.out.print("Enter word to add: ");
                    String word = scanner.nextLine();
                    System.out.print("Enter definition: ");
                    String definition = scanner.nextLine();
                    System.out.print("Enter pronunciation: ");
                    String pronunciation = scanner.nextLine();

                    System.out.print("Enter synonyms (comma-separated): ");
                    String[] synonymsArray = scanner.nextLine().split(",");
                    List<String> synonyms = Arrays.asList(synonymsArray);

                    System.out.print("Enter antonyms (comma-separated): ");
                    String[] antonymsArray = scanner.nextLine().split(",");
                    List<String> antonyms = Arrays.asList(antonymsArray);

                    System.out.print("Enter examples (comma-separated): ");
                    String[] examplesArray = scanner.nextLine().split(",");
                    List<String> examples = Arrays.asList(examplesArray);

                    addEntry(dict, word, definition, synonyms, antonyms, examples, pronunciation);
                    break;
                }
                case 2: {
                    System.out.print("Enter word to remove: ");
                    String word = scanner.nextLine();
                    removeEntry(dict, word);
                    break;
                }
                case 3: {
                    System.out.print("Enter word to search: ");
                    String word = scanner.nextLine();
                    searchWord(dict, word);
                    break;
                }
                case 4: {
                    System.out.print("Enter prefix to list words (leave empty to list all words): ");
                    String prefix = scanner.nextLine();
                    listAllWords(dict, prefix);
                    break;
                }
                case 5:
                    System.out.println("Exiting the program.");
                    scanner.close();
                    return;
                default:
                    System.out.println("Invalid option. Please try again.");
                    break;
            }
        }
    }

    public static void main(String[] args) {
        Dictionary myDictionary = new Dictionary();
        mainMenu(myDictionary);
    }
}
