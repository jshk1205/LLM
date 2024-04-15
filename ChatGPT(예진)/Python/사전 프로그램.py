import json
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

class TrieNode:
    def __init__(self):
        self.children = {}
        self.end_of_word = False
        self.definition = None
        self.synonyms = []
        self.antonyms = []
        self.examples = []
        self.frequency = 0
        self.pronunciation = None

class Dictionary:
    def __init__(self):
        self.root = TrieNode()
        self.log_file = "dictionary_log.json"
        self.load_previous_entries()

    def add_entry(self, word, definition, synonyms=None, antonyms=None, examples=None, pronunciation=None):
        node = self.root
        for char in word:
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]
        node.end_of_word = True
        node.definition = definition
        node.synonyms = synonyms if synonyms else []
        node.antonyms = antonyms if antonyms else []
        node.examples = examples if examples else []
        node.pronunciation = pronunciation
        node.frequency += 1
        self.log_entry("add", word)
        if node.frequency > 1 and node.frequency % 5 == 0:
            self.send_email_notification(f"알림: '{word}'가 자주 사용됩니다.")

    def remove_entry(self, word):
        node = self.root
        for char in word:
            if char not in node.children:
                print(f"'{word}'는 사전에 없습니다.")
                return
            node = node.children[char]
        if not node.end_of_word:
            print(f"'{word}'는 사전에 없습니다.")
            return
        node.frequency -= 1
        self.log_entry("remove", word)
        print(f"'{word}'를 사전에서 삭제했습니다.")

    def search_word(self, word):
        node = self.root
        for char in word:
            if char not in node.children:
                print(f"'{word}'의 정의를 찾을 수 없습니다.")
                return
            node = node.children[char]
        if not node.end_of_word:
            print(f"'{word}'의 정의를 찾을 수 없습니다.")
            return
        print(f"'{word}'의 정의: {node.definition}")
        print(f"발음: {node.pronunciation}")
        print(f"동의어: {', '.join(node.synonyms)}")
        print(f"반의어: {', '.join(node.antonyms)}")
        print("예문:")
        for example in node.examples:
            print(f"- {example}")
        print(f"빈도수: {node.frequency}")

    def list_all_words(self, prefix=""):
        node = self.root
        for char in prefix:
            if char not in node.children:
                print("사전에 있는 모든 단어:")
                return
            node = node.children[char]
        self._list_words_from_node(node, prefix)

    def _list_words_from_node(self, node, prefix):
        if node.end_of_word:
            print(prefix)
        for char, child in node.children.items():
            self._list_words_from_node(child, prefix + char)

    def log_entry(self, action, word):
        with open(self.log_file, 'a') as log:
            log_entry = {
                "action": action,
                "word": word
            }
            log.write(json.dumps(log_entry) + '\n')

    def load_previous_entries(self):
        try:
            with open(self.log_file, 'r') as log:
                for line in log:
                    entry = json.loads(line)
                    if entry["action"] == "add":
                        self.add_entry(entry["word"], None)
                    elif entry["action"] == "remove":
                        self.remove_entry(entry["word"])
        except FileNotFoundError:
            print("이전 로그 파일을 찾을 수 없습니다. 새로운 로그 파일을 생성합니다.")

    def send_email_notification(self, message):
        from_address = "your_email@gmail.com"  # 이메일 보내는 주소
        to_address = "recipient_email@gmail.com"  # 이메일 받는 주소
        password = "your_password"  # 이메일 보내는 주소의 앱 비밀번호

        msg = MIMEMultipart()
        msg['From'] = from_address
        msg['To'] = to_address
        msg['Subject'] = "사전 알림"
        msg.attach(MIMEText(message, 'plain'))

        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        server.login(from_address, password)
        server.send_message(msg)
        server.quit()

# 사용자 인터페이스 함수
def main():
    my_dictionary = Dictionary()
    while True:
        print("\n사전 프로그램")
        print("1. 단어 추가")
        print("2. 단어 삭제")
        print("3. 단어 검색")
        print("4. 모든 단어 출력")
        print("5. 종료")
        choice = input("원하는 작업을 선택하세요: ")

        if choice == '1':
            word = input("추가할 단어: ")
            definition = input("단어의 정의: ")
            pronunciation = input("발음: ")
            synonyms = input("동의어 (쉼표로 구분): ").split(',')
            antonyms = input("반의어 (쉼표로 구분): ").split(',')
            examples = input("예문 (각 예문은 쉼표로 구분): ").split(',')
            my_dictionary.add_entry(word, definition, synonyms, antonyms, examples, pronunciation)

        elif choice == '2':
            word = input("삭제할 단어: ")
            my_dictionary.remove_entry(word)

        elif choice == '3':
            word = input("검색할 단어: ")
            my_dictionary.search_word(word)

        elif choice == '4':
            prefix = input("자동 완성할 접두사를 입력하세요 (비워두면 모든 단어 출력): ")
            my_dictionary.list_all_words(prefix)

        elif choice == '5':
            print("프로그램을 종료합니다.")
            break

        else:
            print("올바른 옵션을 선택하세요.")

if __name__ == "__main__":
    main()
