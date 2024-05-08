import tkinter as tk
from tkinter import messagebox
import os
from datetime import datetime


class MemoApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Memo App")

        self.memo_list = []

        self.load_memos()

        self.create_widgets()

    def create_widgets(self):
        self.label_title = tk.Label(self.master, text="Title:")
        self.label_title.grid(row=0, column=0, sticky="e")

        self.entry_title = tk.Entry(self.master, width=50)
        self.entry_title.grid(row=0, column=1, columnspan=2)

        self.label_content = tk.Label(self.master, text="Content:")
        self.label_content.grid(row=1, column=0, sticky="ne")

        self.text_content = tk.Text(self.master, height=10, width=50)
        self.text_content.grid(row=1, column=1, columnspan=2)

        self.button_save = tk.Button(self.master, text="Save", command=self.save_memo)
        self.button_save.grid(row=2, column=0)

        self.button_search = tk.Button(self.master, text="Search", command=self.search_memo)
        self.button_search.grid(row=2, column=1)

        self.button_delete = tk.Button(self.master, text="Delete", command=self.delete_memo)
        self.button_delete.grid(row=2, column=2)

        self.button_edit = tk.Button(self.master, text="Edit", command=self.edit_memo)
        self.button_edit.grid(row=3, column=0)

    def save_memo(self):
        title = self.entry_title.get()
        content = self.text_content.get("1.0", "end-1c")
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        if title and content:
            self.memo_list.append({"title": title, "content": content, "timestamp": timestamp})
            self.save_to_file()
            messagebox.showinfo("Save", "Memo has been saved.")
            self.clear_entries()
        else:
            messagebox.showerror("Error", "Please enter title and content.")

    def search_memo(self):
        title_to_search = self.entry_title.get()
        for memo in self.memo_list:
            if memo["title"] == title_to_search:
                messagebox.showinfo(memo["title"], memo["content"])
                return
        messagebox.showerror("Error", "Memo not found.")

    def delete_memo(self):
        title_to_delete = self.entry_title.get()
        for memo in self.memo_list:
            if memo["title"] == title_to_delete:
                self.memo_list.remove(memo)
                self.save_to_file()
                messagebox.showinfo("Delete", "Memo has been deleted.")
                self.clear_entries()
                return
        messagebox.showerror("Error", "Memo not found.")

    def edit_memo(self):
        title_to_edit = self.entry_title.get()
        for memo in self.memo_list:
            if memo["title"] == title_to_edit:
                self.memo_list.remove(memo)
                self.text_content.delete("1.0", "end")
                self.entry_title.delete(0, "end")
                self.entry_title.insert(0, memo["title"])
                self.text_content.insert("1.0", memo["content"])
                messagebox.showinfo("Edit", "You can now edit the memo.")
                return
        messagebox.showerror("Error", "Memo not found.")

    def save_to_file(self):
        with open("memos.txt", "w") as file:
            for memo in self.memo_list:
                file.write(f"{memo['title']}\n{memo['content']}\n{memo['timestamp']}\n---\n")

    def load_memos(self):
        if os.path.exists("memos.txt"):
            with open("memos.txt", "r") as file:
                lines = file.readlines()
                memo_data = [lines[i:i + 4] for i in range(0, len(lines), 4)]
                for data in memo_data:
                    title = data[0].strip()
                    content = data[1].strip()
                    timestamp = data[2].strip()
                    self.memo_list.append({"title": title, "content": content, "timestamp": timestamp})

    def clear_entries(self):
        self.entry_title.delete(0, "end")
        self.text_content.delete("1.0", "end")


def main():
    root = tk.Tk()
    app = MemoApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
