#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char item;
    double timestamp;
} StackItem;

typedef struct {
    StackItem* items;
    int top;
    int capacity;
} Stack;

void initStack(Stack* stack, int capacity) {
    stack->items = (StackItem*)malloc(sizeof(StackItem) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
}

void push(Stack* stack, char item) {
    if (stack->top == stack->capacity - 1) {
        stack->capacity *= 2;
        stack->items = (StackItem*)realloc(stack->items, sizeof(StackItem) * stack->capacity);
    }
    stack->top++;
    stack->items[stack->top].item = item;
    stack->items[stack->top].timestamp = (double)clock() / CLOCKS_PER_SEC;
}

StackItem pop(Stack* stack) {
    StackItem item = { '\0', 0.0 };
    if (stack->top != -1) {
        item = stack->items[stack->top];
        stack->top--;
    }
    return item;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

StackItem peek(Stack* stack) {
    StackItem item = { '\0', 0.0 };
    if (stack->top != -1) {
        item = stack->items[stack->top];
    }
    return item;
}

void mainMenu() {
    printf("\n1. Push a character\n");
    printf("2. Pop the last input\n");
    printf("3. Pop the last input in order\n");
    printf("4. Peek the last input\n");
    printf("5. Copy last input to clipboard\n");
    printf("6. Paste from clipboard\n");
    printf("7. Undo last input\n");
    printf("8. Quit\n");
}

void popLastInput(Stack* stack) {
    if (!isEmpty(stack)) {
        StackItem item = pop(stack);
        printf("Pop the last input: %c\n", item.item);
    }
    else {
        printf("Stack is empty.\n");
    }
}

void popLastInputOrdered(Stack* stack) {
    if (!isEmpty(stack)) {
        StackItem* orderedItems = (StackItem*)malloc(sizeof(StackItem) * (stack->top + 1));
        int count = 0;
        while (!isEmpty(stack)) {
            orderedItems[count++] = pop(stack);
        }
        for (int i = count - 1; i >= 0; i--) {
            printf("%c", orderedItems[i].item);
        }
        printf("\n");
        free(orderedItems);
    }
    else {
        printf("Stack is empty.\n");
    }
}

void peekLastInput(Stack* stack) {
    if (!isEmpty(stack)) {
        StackItem item = peek(stack);
        printf("Peek the last input: %c\n", item.item);
    }
    else {
        printf("Stack is empty.\n");
    }
}

void copyToClipboard(Stack* stack, Stack* clipboard) {
    if (!isEmpty(stack)) {
        StackItem item = peek(stack);
        push(clipboard, item.item);
        printf("Last input '%c' copied to clipboard.\n", item.item);
    }
    else {
        printf("Stack is empty.\n");
    }
}

void pasteFromClipboard(Stack* stack, Stack* clipboard) {
    if (!isEmpty(clipboard)) {
        StackItem item = peek(clipboard);
        push(stack, item.item);
        printf("Last input '%c' pasted from clipboard.\n", item.item);
    }
    else {
        printf("Clipboard is empty.\n");
    }
}

void undo(Stack* stack) {
    if (!isEmpty(stack)) {
        StackItem item = pop(stack);
        printf("Undo last input: %c\n", item.item);
    }
    else {
        printf("Stack is empty.\n");
    }
}

int main() {
    Stack stack;
    Stack clipboard;
    initStack(&stack, 10);
    initStack(&clipboard, 10);

    while (1) {
        mainMenu();
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        if (choice == 1) {
            char character;
            printf("Enter a character to push: ");
            scanf("%c", &character);
            getchar(); // Consume the newline character
            push(&stack, character);
        }
        else if (choice == 2) {
            popLastInput(&stack);
        }
        else if (choice == 3) {
            popLastInputOrdered(&stack);
        }
        else if (choice == 4) {
            peekLastInput(&stack);
        }
        else if (choice == 5) {
            copyToClipboard(&stack, &clipboard);
        }
        else if (choice == 6) {
            pasteFromClipboard(&stack, &clipboard);
        }
        else if (choice == 7) {
            undo(&stack);
        }
        else if (choice == 8) {
            printf("Exiting program...\n");
            break;
        }
        else {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    free(stack.items);
    free(clipboard.items);
    return 0;
}
