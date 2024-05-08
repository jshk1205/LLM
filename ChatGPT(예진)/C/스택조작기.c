#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct {
    char items[MAX_SIZE][2];
    int top;
} Stack;

void init(Stack* stack) {
    stack->top = -1;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

int isFull(Stack* stack) {
    return stack->top == MAX_SIZE - 1;
}

void push(Stack* stack, char item) {
    if (!isFull(stack)) {
        stack->top++;
        stack->items[stack->top][0] = item;
        stack->items[stack->top][1] = '\0'; // Null-terminate string
    }
}

char* pop(Stack* stack) {
    if (!isEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return NULL;
}

char* peek(Stack* stack) {
    if (!isEmpty(stack)) {
        return stack->items[stack->top];
    }
    return NULL;
}

void printMenu() {
    printf("\n1. Push a character\n");
    printf("2. Pop the last input\n");
    printf("3. Pop the last input in order\n");
    printf("4. Peek the last input\n");
    printf("5. Copy last input to clipboard\n");
    printf("6. Paste from clipboard\n");
    printf("7. Undo last input\n");
    printf("8. Quit\n");
    printf("Enter your choice: ");
}

void popLastInput(Stack* stack) {
    if (!isEmpty(stack)) {
        printf("Pop the last input: %s\n", pop(stack));
    }
    else {
        printf("Stack is empty.\n");
    }
}

void popLastInputOrdered(Stack* stack) {
    if (!isEmpty(stack)) {
        printf("Pop the last input in order: ");
        while (!isEmpty(stack)) {
            printf("%s", pop(stack));
        }
        printf("\n");
    }
    else {
        printf("Stack is empty.\n");
    }
}

void peekLastInput(Stack* stack) {
    if (!isEmpty(stack)) {
        printf("Peek the last input: %s\n", peek(stack));
    }
    else {
        printf("Stack is empty.\n");
    }
}

void copyToClipboard(Stack* stack, Stack* clipboard) {
    if (!isEmpty(stack)) {
        char* item = peek(stack);
        push(clipboard, item[0]);
        printf("Last input '%s' copied to clipboard.\n", item);
    }
    else {
        printf("Stack is empty.\n");
    }
}

void pasteFromClipboard(Stack* stack, Stack* clipboard) {
    if (!isEmpty(clipboard)) {
        char* item = peek(clipboard);
        push(stack, item[0]);
        printf("Last input '%s' pasted from clipboard.\n", item);
    }
    else {
        printf("Clipboard is empty.\n");
    }
}

void undo(Stack* stack) {
    if (!isEmpty(stack)) {
        pop(stack);
        printf("Undo last input.\n");
    }
    else {
        printf("Stack is empty.\n");
    }
}

int main() {
    Stack stack, clipboard;
    init(&stack);
    init(&clipboard);
    char choice;
    while (1) {
        printMenu();
        scanf(" %c", &choice);

        switch (choice) {
        case '1':
            printf("Enter a character to push: ");
            char character;
            scanf(" %c", &character);
            push(&stack, character);
            break;
        case '2':
            popLastInput(&stack);
            break;
        case '3':
            popLastInputOrdered(&stack);
            break;
        case '4':
            peekLastInput(&stack);
            break;
        case '5':
            copyToClipboard(&stack, &clipboard);
            break;
        case '6':
            pasteFromClipboard(&stack, &clipboard);
            break;
        case '7':
            undo(&stack);
            break;
        case '8':
            printf("Exiting program...\n");
            exit(0);
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}
