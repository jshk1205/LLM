import getpass
from datetime import datetime, timedelta

class User:
    def __init__(self, name, username, password):
        self.name = name
        self.username = username
        self.password = password
        self.diary_entries = {}

class DiaryProgram:
    def __init__(self):
        self.users = {}
        self.current_user = None

    def run(self):
        while True:
            print("1. Login")
            print("2. Sign Up")
            choice = input("Select an option: ")

            if choice == '1':
                self.login()
                if self.current_user:
                    self.main_page()
            elif choice == '2':
                self.sign_up()

    def login(self):
        print("\n--- Login ---")
        username = input("Enter your username: ")
        password = getpass.getpass("Enter your password: ")

        if username in self.users and self.users[username].password == password:
            self.current_user = self.users[username]
            print("Login successful!")
        else:
            print("Invalid username or password.")

    def sign_up(self):
        print("\n--- Sign Up ---")
        name = input("Enter your name: ")
        username = input("Enter a username: ")

        if username.isdigit():
            print("Username cannot be all digits.")
            return

        if username in self.users:
            print("Username already exists.")
            return

        password = getpass.getpass("Enter a password: ")
        if not name or not username or not password:
            print("All fields are required.")
            return

        self.users[username] = User(name, username, password)
        print("Sign up successful!")

    def main_page(self):
        while True:
            print(f"\nWelcome, {self.current_user.name}'s Diary")
            today = datetime.now()
            self.display_calendar(today)

            print("3. My Diary")
            print("4. My Diary List")
            print("5. Logout")

            choice = input("Select an option: ")

            if choice == '3':
                self.diary_page(today)
            elif choice == '4':
                self.diary_list_page()
            elif choice == '5':
                self.current_user = None
                return

    def display_calendar(self, date):
        month = date.month
        year = date.year
        num_days = (date.replace(day=28) + timedelta(days=4)).day

        print(f"\n{date.strftime('%B')} {year}")
        print("  M  T  W  T  F  S  S")

        offset = date.weekday()
        for i in range(offset):
            print("   ", end=" ")

        for day in range(1, num_days + 1):
            if day == datetime.now().day and month == datetime.now().month:
                print(f"\033[91m{day:2}\033[00m ", end=" ")
            else:
                print(f"{day:2} ", end=" ")

            offset += 1
            if offset % 7 == 0:
                print()

    def diary_page(self, date):
        print(f"\n--- Diary Entry: {date.strftime('%B %d, %Y')} ---")
        entry = input("Write your diary entry (Press Enter to finish):\n")
        if entry.strip():
            self.current_user.diary_entries[date] = entry
            print("Entry saved successfully!")
        else:
            print("No entry to save.")

    def diary_list_page(self):
        print("\n--- Diary List ---")
        for date in sorted(self.current_user.diary_entries.keys()):
            print(date.strftime('%B %d, %Y'))

        selected_date = input("Select a date to view entry (MM/DD/YYYY), or type 'back' to return: ")
        if selected_date.lower() == 'back':
            return

        try:
            selected_date = datetime.strptime(selected_date, '%m/%d/%Y')
            if selected_date in self.current_user.diary_entries:
                print(f"\n--- Diary Entry: {selected_date.strftime('%B %d, %Y')} ---")
                print(self.current_user.diary_entries[selected_date])
            else:
                print("No entry found for selected date.")
        except ValueError:
            print("Invalid date format.")

if __name__ == "__main__":
    diary_program = DiaryProgram()
    diary_program.run()
