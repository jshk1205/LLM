import json

class Contact:
    def __init__(self, name, number, email="", address=""):
        self.name = name
        self.number = number
        self.email = email
        self.address = address

    def __repr__(self):
        return f"Name: {self.name}, Number: {self.number}, Email: {self.email}, Address: {self.address}"

class Phonebook:
    def __init__(self):
        self.contacts = {}
        self.groups = {}

    def add_contact(self, contact):
        self.contacts[contact.name] = contact

    def delete_contact(self, name):
        if name in self.contacts:
            del self.contacts[name]
        else:
            print("연락처를 찾을 수 없습니다.")

    def search_contact(self, name):
        if name in self.contacts:
            print(self.contacts[name])
        else:
            print("연락처를 찾을 수 없습니다.")

    def display_contacts(self):
        if self.contacts:
            print("전화번호부:")
            for contact in self.contacts.values():
                print(contact)
        else:
            print("전화번호부가 비어 있습니다.")

    def add_group(self, group_name):
        self.groups[group_name] = []

    def add_to_group(self, name, group_name):
        if name in self.contacts and group_name in self.groups:
            self.groups[group_name].append(name)
        else:
            print("연락처 또는 그룹을 찾을 수 없습니다.")

    def modify_contact(self, name, new_number, new_email, new_address):
        if name in self.contacts:
            contact = self.contacts[name]
            contact.number = new_number
            contact.email = new_email
            contact.address = new_address
            print("연락처가 성공적으로 수정되었습니다.")
        else:
            print("연락처를 찾을 수 없습니다.")

    def save_phonebook(self, filename):
        with open(filename, 'w') as f:
            data = {
                "contacts": {name: {"number": contact.number, "email": contact.email, "address": contact.address} for name, contact in self.contacts.items()},
                "groups": self.groups
            }
            json.dump(data, f)

    def load_phonebook(self, filename):
        try:
            with open(filename, 'r') as f:
                data = json.load(f)
                for name, info in data["contacts"].items():
                    contact = Contact(name, info["number"], info["email"], info["address"])
                    self.contacts[name] = contact
                self.groups = data["groups"]
        except FileNotFoundError:
            print("파일을 찾을 수 없습니다.")


def main():
    phonebook = Phonebook()
    phonebook.load_phonebook("phonebook.json")

    while True:
        print("\n전화번호부 프로그램")
        print("1. 연락처 추가")
        print("2. 연락처 삭제")
        print("3. 연락처 검색")
        print("4. 전체 연락처 보기")
        print("5. 그룹 생성")
        print("6. 그룹에 연락처 추가")
        print("7. 연락처 수정")
        print("8. 전화번호부 저장")
        print("9. 종료")

        choice = input("원하는 작업을 선택하세요: ")

        if choice == '1':
            name = input("이름을 입력하세요: ")
            number = input("전화번호를 입력하세요: ")
            email = input("이메일을 입력하세요 (없으면 엔터): ")
            address = input("주소를 입력하세요 (없으면 엔터): ")
            contact = Contact(name, number, email, address)
            phonebook.add_contact(contact)
        elif choice == '2':
            name = input("삭제할 연락처의 이름을 입력하세요: ")
            phonebook.delete_contact(name)
        elif choice == '3':
            name = input("검색할 연락처의 이름을 입력하세요: ")
            phonebook.search_contact(name)
        elif choice == '4':
            phonebook.display_contacts()
        elif choice == '5':
            group_name = input("생성할 그룹의 이름을 입력하세요: ")
            phonebook.add_group(group_name)
        elif choice == '6':
            name = input("그룹에 추가할 연락처의 이름을 입력하세요: ")
            group_name = input("추가할 그룹의 이름을 입력하세요: ")
            phonebook.add_to_group(name, group_name)
        elif choice == '7':
            name = input("수정할 연락처의 이름을 입력하세요: ")
            new_number = input("새로운 전화번호를 입력하세요: ")
            new_email = input("새로운 이메일을 입력하세요: ")
            new_address = input("새로운 주소를 입력하세요: ")
            phonebook.modify_contact(name, new_number, new_email, new_address)
        elif choice == '8':
            phonebook.save_phonebook("phonebook.json")
            print("전화번호부가 저장되었습니다.")
        elif choice == '9':
            print("프로그램을 종료합니다.")
            break
        else:
            print("잘못된 선택입니다. 다시 선택하세요.")


if __name__ == "__main__":
    main()
