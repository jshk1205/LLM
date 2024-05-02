#include <iostream>
#include <stack>
#include <string>

#define MAX_SIZE 100

void printMenu() {
    std::cout << "\n1. Push a character\n"
        << "2. Pop the last input\n"
        << "3. Pop the last input in order\n"
        << "4. Peek the last input\n"
        << "5. Copy last input to clipboard\n"
        << "6. Paste from clipboard\n"
        << "7. Undo last input\n"
        << "8. Quit\n"
        << "Enter your choice: ";
}

void popLastInput(std::stack<std::string>& stack) {
    if (!stack.empty()) {
        std::cout << "Pop the last input: " << stack.top() << std::endl;
        stack.pop();
    }
    else {
        std::cout << "Stack is empty." << std::endl;
    }
}

void popLastInputOrdered(std::stack<std::string>& stack) {
    if (!stack.empty()) {
        std::cout << "Pop the last input in order: ";
        while (!stack.empty()) {
            std::cout << stack.top();
            stack.pop();
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Stack is empty." << std::endl;
    }
}

void peekLastInput(std::stack<std::string>& stack) {
    if (!stack.empty()) {
        std::cout << "Peek the last input: " << stack.top() << std::endl;
    }
    else {
        std::cout << "Stack is empty." << std::endl;
    }
}

void copyToClipboard(std::stack<std::string>& stack, std::stack<std::string>& clipboard) {
    if (!stack.empty()) {
        clipboard.push(stack.top());
        std::cout << "Last input '" << stack.top() << "' copied to clipboard." << std::endl;
    }
    else {
        std::cout << "Stack is empty." << std::endl;
    }
}

void pasteFromClipboard(std::stack<std::string>& stack, std::stack<std::string>& clipboard) {
    if (!clipboard.empty()) {
        stack.push(clipboard.top());
        std::cout << "Last input '" << clipboard.top() << "' pasted from clipboard." << std::endl;
    }
    else {
        std::cout << "Clipboard is empty." << std::endl;
    }
}

void undo(std::stack<std::string>& stack) {
    if (!stack.empty()) {
        stack.pop();
        std::cout << "Undo last input." << std::endl;
    }
    else {
        std::cout << "Stack is empty." << std::endl;
    }
}

int main() {
    std::stack<std::string> stack, clipboard;
    char choice;
    while (true) {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case '1': {
            std::cout << "Enter a character to push: ";
            char character;
            std::cin >> character;
            stack.push(std::string(1, character));
            break;
        }
        case '2':
            popLastInput(stack);
            break;
        case '3':
            popLastInputOrdered(stack);
            break;
        case '4':
            peekLastInput(stack);
            break;
        case '5':
            copyToClipboard(stack, clipboard);
            break;
        case '6':
            pasteFromClipboard(stack, clipboard);
            break;
        case '7':
            undo(stack);
            break;
        case '8':
            std::cout << "Exiting program..." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please enter a valid option." << std::endl;
        }
    }

    return 0;
}
