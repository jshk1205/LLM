import java.util.Scanner;
import java.util.Stack;

class StackWithTimestamp {
    private Stack<ItemWithTimestamp> items;

    public StackWithTimestamp() {
        this.items = new Stack<>();
    }

    public void push(String item) {
        long timestamp = System.currentTimeMillis();
        items.push(new ItemWithTimestamp(item, timestamp));
    }

    public ItemWithTimestamp pop() {
        if (!isEmpty()) {
            return items.pop();
        }
        return null;
    }

    public boolean isEmpty() {
        return items.isEmpty();
    }

    public ItemWithTimestamp peek() {
        if (!isEmpty()) {
            return items.peek();
        }
        return null;
    }

    public static class ItemWithTimestamp {
        String item;
        long timestamp;

        public ItemWithTimestamp(String item, long timestamp) {
            this.item = item;
            this.timestamp = timestamp;
        }

        public String getItem() {
            return item;
        }

        public long getTimestamp() {
            return timestamp;
        }
    }
}

public class Stack_Manipulator {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        StackWithTimestamp stack = new StackWithTimestamp();
        StackWithTimestamp clipboard = new StackWithTimestamp();  // 클립보드 스택 생성

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

    public static void popLastInput(StackWithTimestamp stack) {
        if (!stack.isEmpty()) {
            StackWithTimestamp.ItemWithTimestamp item = stack.pop();
            System.out.println("Pop the last input: " + item.getItem());
        } else {
            System.out.println("Stack is empty.");
        }
    }

    public static void popLastInputOrdered(StackWithTimestamp stack) {
        if (!stack.isEmpty()) {
            StringBuilder orderedInputs = new StringBuilder();
            while (!stack.isEmpty()) {
                StackWithTimestamp.ItemWithTimestamp item = stack.pop(); // 타임스탬프를 무시하고 아이템만 가져옴
                orderedInputs.append(item.getItem());
            }
            System.out.println("Pop the last input in order: " + orderedInputs.toString());
        } else {
            System.out.println("Stack is empty.");
        }
    }

    public static void peekLastInput(StackWithTimestamp stack) {
        if (!stack.isEmpty()) {
            StackWithTimestamp.ItemWithTimestamp item = stack.peek();
            System.out.println("Peek the last input: " + item.getItem());
        } else {
            System.out.println("Stack is empty.");
        }
    }

    public static void copyToClipboard(StackWithTimestamp stack, StackWithTimestamp clipboard) {
        if (!stack.isEmpty()) {
            StackWithTimestamp.ItemWithTimestamp item = stack.peek();
            clipboard.push(item.getItem());
            System.out.println("Last input '" + item.getItem() + "' copied to clipboard.");
        } else {
            System.out.println("Stack is empty.");
        }
    }

    public static void pasteFromClipboard(StackWithTimestamp stack, StackWithTimestamp clipboard) {
        if (!clipboard.isEmpty()) {
            StackWithTimestamp.ItemWithTimestamp item = clipboard.peek();
            stack.push(item.getItem());
            System.out.println("Last input '" + item.getItem() + "' pasted from clipboard.");
        } else {
            System.out.println("Clipboard is empty.");
        }
    }

    public static void undo(StackWithTimestamp stack) {
        if (!stack.isEmpty()) {
            StackWithTimestamp.ItemWithTimestamp item = stack.pop();
            System.out.println("Undo last input: " + item.getItem());
        } else {
            System.out.println("Stack is empty.");
        }
    }
}
