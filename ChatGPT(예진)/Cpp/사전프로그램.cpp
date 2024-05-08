#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool end_of_word;
    std::string definition;
    std::vector<std::string> synonyms;
    std::vector<std::string> antonyms;
    std::vector<std::string> examples;
    int frequency;
    std::string pronunciation;

    TrieNode() {
        end_of_word = false;
        frequency = 0;
    }
};

class Dictionary {
private:
    TrieNode* root;
    std::string log_file;

public:
    Dictionary() {
        root = new TrieNode();
        log_file = "dictionary_log.txt";
        load_previous_entries();
    }

    void add_entry(std::string word, std::string definition, std::vector<std::string> synonyms = {}, std::vector<std::string> antonyms = {}, std::vector<std::string> examples = {}, std::string pronunciation = "") {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->end_of_word = true;
        node->definition = definition;
        node->synonyms = synonyms;
        node->antonyms = antonyms;
        node->examples = examples;
        node->pronunciation = pronunciation;
        node->frequency++;
        log_entry("add", word);
        if (node->frequency > 1 && node->frequency % 5 == 0) {
            // 이메일 알림 보내기
            send_email_notification("알림: " + word + "가 자주 사용됩니다.");
        }
    }

    void remove_entry(std::string word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                std::cout << "'" << word << "'는 사전에 없습니다." << std::endl;
                return;
            }
            node = node->children[c];
        }
        if (!node->end_of_word) {
            std::cout << "'" << word << "'는 사전에 없습니다." << std::endl;
            return;
        }
        node->frequency--;
        log_entry("remove", word);
        std::cout << "'" << word << "'를 사전에서 삭제했습니다." << std::endl;
    }

    void search_word(std::string word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                std::cout << "'" << word << "'의 정의를 찾을 수 없습니다." << std::endl;
                return;
            }
            node = node->children[c];
        }
        if (!node->end_of_word) {
            std::cout << "'" << word << "'의 정의를 찾을 수 없습니다." << std::endl;
            return;
        }
        std::cout << "'" << word << "'의 정의: " << node->definition << std::endl;
        std::cout << "발음: " << node->pronunciation << std::endl;
        std::cout << "동의어: ";
        for (const auto& synonym : node->synonyms) {
            std::cout << synonym << ", ";
        }
        std::cout << std::endl;
        std::cout << "반의어: ";
        for (const auto& antonym : node->antonyms) {
            std::cout << antonym << ", ";
        }
        std::cout << std::endl << "예문:" << std::endl;
        for (const auto& example : node->examples) {
            std::cout << "- " << example << std::endl;
        }
        std::cout << "빈도수: " << node->frequency << std::endl;
    }

    void list_all_words(std::string prefix = "") {
        TrieNode* node = root;
        for (char c : prefix) {
            if (node->children.find(c) == node->children.end()) {
                std::cout << "사전에 있는 모든 단어:" << std::endl;
                return;
            }
            node = node->children[c];
        }
        _list_words_from_node(node, prefix);
    }

private:
    void _list_words_from_node(TrieNode* node, std::string prefix) {
        if (node->end_of_word) {
            std::cout << prefix << std::endl;
        }
        for (const auto& child : node->children) {
            _list_words_from_node(child.second, prefix + child.first);
        }
    }

    void log_entry(std::string action, std::string word) {
        std::ofstream log(log_file, std::ios_base::app);
        if (log.is_open()) {
            log << "{\"action\": \"" << action << "\", \"word\": \"" << word << "\"}" << std::endl;
            log.close();
        }
    }

    void load_previous_entries() {
        std::ifstream log(log_file);
        if (log.is_open()) {
            std::string line;
            while (std::getline(log, line)) {
                std::istringstream iss(line);
                std::string action, word;
                iss >> action >> word;
                if (action == "add") {
                    add_entry(word, "");
                }
                else if (action == "remove") {
                    remove_entry(word);
                }
            }
            log.close();
        }
        else {
            std::cout << "이전 로그 파일을 찾을 수 없습니다. 새로운 로그 파일을 생성합니다." << std::endl;
        }
    }

    void send_email_notification(std::string message) {
        // 여기에 이메일 전송 코드를 추가하세요
    }
};

void main_menu(Dictionary& my_dictionary) {
    while (true) {
        std::cout << "\n사전 프로그램" << std::endl;
        std::cout << "1. 단어 추가" << std::endl;
        std::cout << "2. 단어 삭제" << std::endl;
        std::cout << "3. 단어 검색" << std::endl;
        std::cout << "4. 모든 단어 출력" << std::endl;
        std::cout << "5. 종료" << std::endl;
        std::cout << "원하는 작업을 선택하세요: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string word, definition, pronunciation;
            std::vector<std::string> synonyms, antonyms, examples;
            std::cout << "추가할 단어: ";
            std::cin >> word;
            std::cout << "단어의 정의: ";
            std::cin.ignore(); // Ignore newline character
            std::getline(std::cin, definition);
            std::cout << "발음: ";
            std::cin >> pronunciation;
            std::cout << "동의어 (쉼표로 구분): ";
            std::string synonyms_input;
            std::cin.ignore(); // Ignore newline character
            std::getline(std::cin, synonyms_input);
            std::istringstream iss(synonyms_input);
            std::string synonym;
            while (std::getline(iss, synonym, ',')) {
                synonyms.push_back(synonym);
            }
            std::cout << "반의어 (쉼표로 구분): ";
            std::string antonyms_input;
            std::getline(std::cin, antonyms_input);
            std::istringstream iss_ant(antonyms_input);
            std::string antonym;
            while (std::getline(iss_ant, antonym, ',')) {
                antonyms.push_back(antonym);
            }
            std::cout << "예문 (각 예문은 쉼표로 구분): ";
            std::string examples_input;
            std::getline(std::cin, examples_input);
            std::istringstream iss_ex(examples_input);
            std::string example;
            while (std::getline(iss_ex, example, ',')) {
                examples.push_back(example);
            }
            my_dictionary.add_entry(word, definition, synonyms, antonyms, examples, pronunciation);
            break;
        }
        case 2: {
            std::string word;
            std::cout << "삭제할 단어: ";
            std::cin >> word;
            my_dictionary.remove_entry(word);
            break;
        }
        case 3: {
            std::string word;
            std::cout << "검색할 단어: ";
            std::cin >> word;
            my_dictionary.search_word(word);
            break;
        }
        case 4: {
            std::string prefix;
            std::cout << "자동 완성할 접두사를 입력하세요 (비워두면 모든 단어 출력): ";
            std::cin >> prefix;
            my_dictionary.list_all_words(prefix);
            break;
        }
        case 5:
            std::cout << "프로그램을 종료합니다." << std::endl;
            return;
        default:
            std::cout << "올바른 옵션을 선택하세요." << std::endl;
            break;
        }
    }
}

int main() {
    Dictionary my_dictionary;
    main_menu(my_dictionary);
    return 0;
}
