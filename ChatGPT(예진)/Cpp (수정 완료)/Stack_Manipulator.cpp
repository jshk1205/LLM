#include <iostream>
#include <vector>
#include <chrono>

struct StackItem {
    char item;
    double timestamp;
};

class Stack {
public:
    Stack(int capacity = 10) : items(), capacity(capacity) {
        items.reserve(capacity);
    }

    void push(char item) {
        if (items.size() == capacity) {
            capacity *= 2;
            items.reserve(capacity);
        }
        double timestamp = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count()) / 1000.0;
        items.push_back({ item, timestamp });
    }

    StackItem pop() {
        if (!isEmpty()) {
            StackItem item = items.back();
            items.pop_back();
            return item;
        }
        return { '\0', 0.0 };
    }

    bool isEmpty() const {
        return items.empty();
    }

    StackItem peek() const {
        if (!isEmpty()) {
            return items.back();
        }
        return { '\0', 0.0 };
    }

private:
    std::vector<StackItem> items;
    int capacity;
};

void mainMenu() {
    std::cout << "\n1. Push a character\n";
    std::cout << "2. Pop the last input\n";
    std::cout << "3. Pop the last input in order\n";
    std::cout << "4. Peek the last input\n";
    std::cout << "5. Copy last input to clipboard\n";
    std::cout << "6. Paste from clipboard\n";
    std::cout << "7. Undo last input\n";
    std::cout << "8. Quit\n";
}

void popLastInput(Stack& stack) {
    if (!stack.isEmpty()) {
        StackItem item = stack.pop();
        std::cout << "Pop the last input: " << item.item << '\n';
    }
    else {
        std::cout << "Stack is empty.\n";
    }
}

void popLastInputOrdered(Stack& stack) {
    if (!stack.isEmpty()) {
        std::vector<StackItem> orderedItems;
        while (!stack.isEmpty()) {
            orderedItems.push_back(stack.pop());
        }
        for (auto it = orderedItems.rbegin(); it != orderedItems.rend(); ++it) {
            std::cout << it->item;
        }
        std::cout << '\n';
    }
    else {
        std::cout << "Stack is empty.\n";
    }
}

void peekLastInput(const Stack& stack) {
    if (!stack.isEmpty()) {
        StackItem item = stack.peek();
        std::cout << "Peek the last input: " << item.item << '\n';
    }
    else {
        std::cout << "Stack is empty.\n";
    }
}

void copyToClipboard(const Stack& stack, Stack& clipboard) {
    if (!stack.isEmpty()) {
        StackItem item = stack.peek();
        clipboard.push(item.item);
        std::cout << "Last input '" << item.item << "' copied to clipboard.\n";
    }
    else {
        std::cout << "Stack is empty.\n";
    }
}

void pasteFromClipboard(Stack& stack, const Stack& clipboard) {
    if (!clipboard.isEmpty()) {
        StackItem item = clipboard.peek();
        stack.push(item.item);
        std::cout << "Last input '" << item.item << "' pasted from clipboard.\n";
    }
    else {
        std::cout << "Clipboard is empty.\n";
    }
}

void undo(Stack& stack) {
    if (!stack.isEmpty()) {
        StackItem item = stack.pop();
        std::cout << "Undo last input: " << item.item << '\n';
    }
    else {
        std::cout << "Stack is empty.\n";
    }
}

int main() {
    Stack stack;
    Stack clipboard;

    while (true) {
        mainMenu();
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            char character;
            std::cout << "Enter a character to push: ";
            std::cin >> character;
            stack.push(character);
        }
        else if (choice == 2) {
            popLastInput(stack);
        }
        else if (choice == 3) {
            popLastInputOrdered(stack);
        }
        else if (choice == 4) {
            peekLastInput(stack);
        }
        else if (choice == 5) {
            copyToClipboard(stack, clipboard);
        }
        else if (choice == 6) {
            pasteFromClipboard(stack, clipboard);
        }
        else if (choice == 7) {
            undo(stack);
        }
        else if (choice == 8) {
            std::cout << "Exiting program...\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}
