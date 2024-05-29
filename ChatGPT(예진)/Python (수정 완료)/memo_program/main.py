import tkinter as tk
from tkinter import messagebox
import psycopg2
from psycopg2 import sql, OperationalError
from datetime import datetime

# search 버튼을 누르면 메모 앱의 화면이 아닌 파이썬 콘솔 창에 리스트가 입력됩니다!
# 메모를 delete하고 싶은 경우, 삭제하고 싶은 메모의 title을 입력한 후 delete 버튼을 눌러야 삭제됩니다.
# edit 버튼도 delete와 동일하게, 같은 title을 입력한 후 content 내용을 수정하면 내용이 바꾼 형태로 저장됩니다.

class MemoApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Memo App")

        # 초기 연결 설정
        self.conn = self.create_connection("postgres", autocommit=True)
        self.create_database("memos_db")
        self.conn.close()

        # 실제 애플리케이션 연결 설정
        self.conn = self.create_connection("memos_db")
        self.cur = self.conn.cursor()
        self.create_table()

        self.memo_list = self.load_memos()

        self.create_widgets()

    def create_connection(self, dbname, autocommit=False):
        try:
            conn = psycopg2.connect(
                dbname=dbname,
                user="myuser",
                password="mypassword",
                host="localhost",
                port="5432"
            )
            if autocommit:
                conn.set_isolation_level(psycopg2.extensions.ISOLATION_LEVEL_AUTOCOMMIT)
            return conn
        except OperationalError as e:
            messagebox.showerror("Database Connection Error", str(e))
            raise e

    def create_database(self, dbname):
        self.cur = self.conn.cursor()
        self.cur.execute(sql.SQL("SELECT 1 FROM pg_catalog.pg_database WHERE datname = %s"), [dbname])
        exists = self.cur.fetchone()
        if not exists:
            self.cur.execute(sql.SQL("CREATE DATABASE {}").format(sql.Identifier(dbname)))
        self.cur.close()

    def create_table(self):
        self.cur.execute("""
        CREATE TABLE IF NOT EXISTS memos (
            id SERIAL PRIMARY KEY,
            title VARCHAR(255) NOT NULL,
            content TEXT NOT NULL,
            timestamp TIMESTAMP NOT NULL
        )
        """)
        self.conn.commit()

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
        timestamp = datetime.now()

        if title and content:
            self.cur.execute("""
            INSERT INTO memos (title, content, timestamp)
            VALUES (%s, %s, %s)
            """, (title, content, timestamp))
            self.conn.commit()
            messagebox.showinfo("Save", "Memo has been saved.")
            self.clear_entries()
        else:
            messagebox.showerror("Error", "Please enter title and content.")

    def search_memo(self):
        title_to_search = self.entry_title.get()
        self.cur.execute("SELECT title, content FROM memos WHERE title = %s", (title_to_search,))
        memo = self.cur.fetchone()
        if memo:
            messagebox.showinfo(memo[0], memo[1])
        else:
            messagebox.showinfo("Check", "Memo list has been printed on console. If you want to delete or modify a memo, please write the same title.")

        # 콘솔에 메모 리스트 출력
        self.cur.execute("SELECT title, content FROM memos")
        memos = self.cur.fetchall()
        for m in memos:
            print(f"Title: {m[0]}, Content: {m[1]}")

        # 메모 리스트 출력 후 커서를 초기 위치로 되돌립니다.
        self.cur.scroll(0, "absolute")

    def delete_memo(self):
        title_to_delete = self.entry_title.get()
        self.cur.execute("DELETE FROM memos WHERE title = %s", (title_to_delete,))
        if self.cur.rowcount:
            self.conn.commit()
            messagebox.showinfo("Delete", "Memo has been deleted.")
            self.memo_list = self.load_memos()  # 메모 리스트 갱신
            self.clear_entries()
        else:
            messagebox.showerror("Error", "Memo not found.")

    def edit_memo(self):
        title_to_edit = self.entry_title.get()
        new_title = self.entry_title.get()  # 새로운 제목
        new_content = self.text_content.get("1.0", "end-1c")  # 새로운 내용
        self.cur.execute("UPDATE memos SET title = %s, content = %s WHERE title = %s",
                         (new_title, new_content, title_to_edit))
        if self.cur.rowcount:
            self.conn.commit()
            messagebox.showinfo("Edit", "Memo has been edited.")
            self.memo_list = self.load_memos()  # 메모 리스트 갱신
            self.clear_entries()
        else:
            messagebox.showerror("Error", "Memo not found.")

    def load_memos(self):
        self.cur.execute("SELECT title, content, timestamp FROM memos")
        return self.cur.fetchall()

    def clear_entries(self):
        self.entry_title.delete(0, "end")
        self.text_content.delete("1.0", "end")

    def __del__(self):
        if hasattr(self, 'cur'):
            self.cur.close()
        if hasattr(self, 'conn'):
            self.conn.close()

def main():
    root = tk.Tk()
    app = MemoApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
