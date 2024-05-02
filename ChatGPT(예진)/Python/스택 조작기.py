from collections import deque
import time

class Stack:
    def __init__(self):
        self.items = deque()

    def push(self, item):
        timestamp = time.time()  # 현재 시간을 밀리초 단위로 저장
        self.items.append((item, timestamp))

    def pop(self):
        if not self.is_empty():
            return self.items.pop()

    def is_empty(self):
        return len(self.items) == 0

    def peek(self):
        if not self.is_empty():
            return self.items[-1]

def main():
    stack = Stack()
    clipboard = Stack()  # 클립보드 스택 생성
    while True:
        print("\n1. Push a character")
        print("2. Pop the last input")
        print("3. Pop the last input in order")
        print("4. Peek the last input")
        print("5. Copy last input to clipboard")
        print("6. Paste from clipboard")
        print("7. Undo last input")
        print("8. Quit")
        choice = input("Enter your choice: ")

        if choice == '1':
            character = input("Enter a character to push: ")
            stack.push(character)
        elif choice == '2':
            popLastInput(stack)
        elif choice == '3':
            popLastInputOrdered(stack)
        elif choice == '4':
            peekLastInput(stack)
        elif choice == '5':
            copyToClipboard(stack, clipboard)
        elif choice == '6':
            pasteFromClipboard(stack, clipboard)
        elif choice == '7':
            undo(stack)
        elif choice == '8':
            print("Exiting program...")
            break
        else:
            print("Invalid choice. Please enter a valid option.")

def popLastInput(stack):
    if not stack.is_empty():
        item, _ = stack.pop()
        print(f"Pop the last input: {item}")
    else:
        print("Stack is empty.")

def popLastInputOrdered(stack):
    if not stack.is_empty():
        ordered_inputs = ""
        while not stack.is_empty():
            item, _ = stack.pop()
            ordered_inputs += item
        print(f"Pop the last input in order: {ordered_inputs}")
    else:
        print("Stack is empty.")

def peekLastInput(stack):
    if not stack.is_empty():
        item, _ = stack.peek()
        print(f"Peek the last input: {item}")
    else:
        print("Stack is empty.")

def copyToClipboard(stack, clipboard):
    if not stack.is_empty():
        item, _ = stack.peek()
        clipboard.push(item)
        print(f"Last input '{item}' copied to clipboard.")
    else:
        print("Stack is empty.")

def pasteFromClipboard(stack, clipboard):
    if not clipboard.is_empty():
        item, _ = clipboard.peek()
        stack.push(item)
        print(f"Last input '{item}' pasted from clipboard.")
    else:
        print("Clipboard is empty.")

def undo(stack):
    if not stack.is_empty():
        _, timestamp = stack.pop()
        print("Undo last input.")
    else:
        print("Stack is empty.")

if __name__ == "__main__":
    main()
