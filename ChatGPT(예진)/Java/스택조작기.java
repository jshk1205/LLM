import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Scanner;

public class 스택조작기 {
    static class Stack {
        private Deque<Pair<String, Long>> items;

        public Stack() {
            items = new ArrayDeque<>();
        }

        public void push(String item) {
            long timestamp = System.currentTimeMillis();
            items.addLast(new Pair<>(item, timestamp));
        }

        public Pair<String, Long> pop() {
            if (!isEmpty()) {
                return items.pollLast();
            }
            return null;
        }

        public boolean isEmpty() {
            return items.isEmpty();
        }

        public Pair<String, Long> peek() {
            if (!isEmpty()) {
                return items.peekLast();
            }
            return null;
        }
    }

    public static void main(String[] args) {
        Stack stack = new Stack();
        Stack clipboard = new Stack();

        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("\n1. Push a character");
            System.out.println("2. Pop the last input");
            System.out.println("3. Pop the last input in order");
            System.out.println("4. Peek the last input");
            System.out.println("5. Copy last input to clipboard");
            System.out.println("6. Paste from clipboard");
            System.out.println("7. Undo last input");
            System.out.println("8. Quit");
            System.out.print("Enter your choice: ");
            String choice = scanner.nextLine();

            switch (choice) {
                case "1":
                    System.out.print("Enter a character to push: ");
                    String character = scanner.nextLine();
                    stack.push(character);
                    break;
                case "2":
                    popLastInput(stack);
                    break;
                case "3":
                    popLastInputOrdered(stack);
                    break;
                case "4":
                    peekLastInput(stack);
                    break;
                case "5":
                    copyToClipboard(stack, clipboard);
                    break;
                case "6":
                    pasteFromClipboard(stack, clipboard);
                    break;
                case "7":
                    undo(stack);
                    break;
                case "8":
                    System.out.println("Exiting program...");
                    scanner.close();
                    return;
                default:
                    System.out.println("Invalid choice. Please enter a valid option.");
            }
        }
    }

    static void popLastInput(Stack stack) {
        Pair<String, Long> item = stack.pop();
        if (item != null) {
            System.out.println("Pop the last input: " + item.getFirst());
        } else {
            System.out.println("Stack is empty.");
        }
    }

    static void popLastInputOrdered(Stack stack) {
        StringBuilder orderedInputs = new StringBuilder();
        while (!stack.isEmpty()) {
            Pair<String, Long> item = stack.pop();
            orderedInputs.insert(0, item.getFirst());
        }
        if (orderedInputs.length() > 0) {
            System.out.println("Pop the last input in order: " + orderedInputs.toString());
        } else {
            System.out.println("Stack is empty.");
        }
    }

    static void peekLastInput(Stack stack) {
        Pair<String, Long> item = stack.peek();
        if (item != null) {
            System.out.println("Peek the last input: " + item.getFirst());
        } else {
            System.out.println("Stack is empty.");
        }
    }

    static void copyToClipboard(Stack stack, Stack clipboard) {
        Pair<String, Long> item = stack.peek();
        if (item != null) {
            clipboard.push(item.getFirst());
            System.out.println("Last input '" + item.getFirst() + "' copied to clipboard.");
        } else {
            System.out.println("Stack is empty.");
        }
    }

    static void pasteFromClipboard(Stack stack, Stack clipboard) {
        Pair<String, Long> item = clipboard.peek();
        if (item != null) {
            stack.push(item.getFirst());
            System.out.println("Last input '" + item.getFirst() + "' pasted from clipboard.");
        } else {
            System.out.println("Clipboard is empty.");
        }
    }

    static void undo(Stack stack) {
        Pair<String, Long> item = stack.pop();
        if (item != null) {
            System.out.println("Undo last input.");
        } else {
            System.out.println("Stack is empty.");
        }
    }

    static class Pair<A, B> {
        private A first;
        private B second;

        public Pair(A first, B second) {
            this.first = first;
            this.second = second;
        }

        public A getFirst() {
            return first;
        }

        public B getSecond() {
            return second;
        }
    }
}
