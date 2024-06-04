#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 4번 모든 단어 출력 부분에서 기능 오류 발생

// 트라이 노드 구조체 정의
typedef struct TrieNode {
    struct TrieNode* children[26]; // 알파벳 소문자 26개
    bool end_of_word;
    char* definition;
    char** synonyms;
    int synonyms_count;
    char** antonyms;
    int antonyms_count;
    char** examples;
    int examples_count;
    int frequency;
    char* pronunciation;
} TrieNode;

// 사전 구조체 정의
typedef struct Dictionary {
    TrieNode* root;
    char* log_file;
} Dictionary;

// 새로운 TrieNode 생성
TrieNode* create_trie_node() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    node->end_of_word = false;
    node->definition = NULL;
    node->synonyms = NULL;
    node->synonyms_count = 0;
    node->antonyms = NULL;
    node->antonyms_count = 0;
    node->examples = NULL;
    node->examples_count = 0;
    node->frequency = 0;
    node->pronunciation = NULL;
    return node;
}

// 사전 초기화
void init_dictionary(Dictionary* dict) {
    dict->root = create_trie_node();
    dict->log_file = "dictionary_log.txt";
    // 로그 파일에서 이전 항목 로드 생략 (C에서는 간단하게 구현하기 어려움)
}

// 트라이에 단어 추가
void add_entry(Dictionary* dict, const char* word, const char* definition, char** synonyms, int synonyms_count, char** antonyms, int antonyms_count, char** examples, int examples_count, const char* pronunciation) {
    TrieNode* node = dict->root;
    while (*word) {
        int index = *word - 'a';
        if (node->children[index] == NULL) {
            node->children[index] = create_trie_node();
        }
        node = node->children[index];
        word++;
    }
    node->end_of_word = true;
    node->definition = _strdup(definition);
    node->synonyms = synonyms;
    node->synonyms_count = synonyms_count;
    node->antonyms = antonyms;
    node->antonyms_count = antonyms_count;
    node->examples = examples;
    node->examples_count = examples_count;
    node->pronunciation = _strdup(pronunciation);
    node->frequency++;
}

// 트라이에서 단어 삭제
void remove_entry(Dictionary* dict, const char* word) {
    TrieNode* node = dict->root;
    while (*word) {
        int index = *word - 'a';
        if (node->children[index] == NULL) {
            printf("'%s'는 사전에 없습니다.\n", word);
            return;
        }
        node = node->children[index];
        word++;
    }
    if (!node->end_of_word) {
        printf("'%s'는 사전에 없습니다.\n", word);
        return;
    }
    node->end_of_word = false;
    node->frequency--;
    printf("사전에서 삭제했습니다.\n", word);
}

// 트라이에서 단어 검색
void search_word(Dictionary* dict, const char* word) {
    TrieNode* node = dict->root;
    while (*word) {
        int index = *word - 'a';
        if (node->children[index] == NULL) {
            printf("'%s'의 정의를 찾을 수 없습니다.\n", word);
            return;
        }
        node = node->children[index];
        word++;
    }
    if (!node->end_of_word) {
        printf("'%s'의 정의를 찾을 수 없습니다.\n", word);
        return;
    }
    printf("'%s'의 정의: %s\n", word, node->definition);
    printf("발음: %s\n", node->pronunciation);
    printf("동의어: ");
    for (int i = 0; i < node->synonyms_count; i++) {
        printf("%s, ", node->synonyms[i]);
    }
    printf("\n반의어: ");
    for (int i = 0; i < node->antonyms_count; i++) {
        printf("%s, ", node->antonyms[i]);
    }
    printf("\n예문:\n");
    for (int i = 0; i < node->examples_count; i++) {
        printf("- %s\n", node->examples[i]);
    }
    printf("빈도수: %d\n", node->frequency);
}

// 특정 노드에서 시작하여 모든 단어 나열
void _list_words_from_node(TrieNode* node, char* prefix, int length) {
    if (node->end_of_word) {
        prefix[length] = '\0';
        printf("%s\n", prefix);
    }
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != NULL) {
            prefix[length] = 'a' + i;
            _list_words_from_node(node->children[i], prefix, length + 1);
        }
    }
}

// 모든 단어 나열
void list_all_words(Dictionary* dict, const char* prefix) {
    TrieNode* node = dict->root;
    while (*prefix) {
        int index = *prefix - 'a';
        if (node->children[index] == NULL) {
            printf("사전에 있는 모든 단어:\n");
            return;
        }
        node = node->children[index];
        prefix++;
    }
    char buffer[100];
    strncpy(buffer, prefix, 100);
    _list_words_from_node(node, buffer, strlen(prefix));
}

// 메인 메뉴 함수
void main_menu(Dictionary* dict) {
    while (true) {
        printf("\n사전 프로그램\n");
        printf("1. 단어 추가\n");
        printf("2. 단어 삭제\n");
        printf("3. 단어 검색\n");
        printf("4. 모든 단어 출력\n");
        printf("5. 종료\n");
        printf("원하는 작업을 선택하세요: ");

        int choice;
        scanf("%d", &choice);
        while (getchar() != '\n');  // 입력 버퍼 비우기

        switch (choice) {
        case 1: {
            char word[100], definition[256], pronunciation[100];
            printf("추가할 단어: ");
            scanf("%99s", word);
            while (getchar() != '\n');
            printf("단어의 정의: ");
            fgets(definition, 256, stdin);
            definition[strcspn(definition, "\n")] = '\0';
            printf("발음: ");
            scanf("%99s", pronunciation);
            while (getchar() != '\n');

            printf("동의어 (쉼표로 구분): ");
            char synonyms_input[256];
            fgets(synonyms_input, 256, stdin);
            synonyms_input[strcspn(synonyms_input, "\n")] = '\0';
            char* synonyms[50];
            int synonyms_count = 0;
            char* token = strtok(synonyms_input, ",");
            while (token != NULL) {
                synonyms[synonyms_count++] = _strdup(token);
                token = strtok(NULL, ",");
            }

            printf("반의어 (쉼표로 구분): ");
            char antonyms_input[256];
            fgets(antonyms_input, 256, stdin);
            antonyms_input[strcspn(antonyms_input, "\n")] = '\0';
            char* antonyms[50];
            int antonyms_count = 0;
            token = strtok(antonyms_input, ",");
            while (token != NULL) {
                antonyms[antonyms_count++] = _strdup(token);
                token = strtok(NULL, ",");
            }

            printf("예문 (각 예문은 쉼표로 구분): ");
            char examples_input[256];
            fgets(examples_input, 256, stdin);
            examples_input[strcspn(examples_input, "\n")] = '\0';
            char* examples[50];
            int examples_count = 0;
            token = strtok(examples_input, ",");
            while (token != NULL) {
                examples[examples_count++] = _strdup(token);
                token = strtok(NULL, ",");
            }

            add_entry(dict, word, definition, synonyms, synonyms_count, antonyms, antonyms_count, examples, examples_count, pronunciation);
            break;
        }
        case 2: {
            char word[100];
            printf("삭제할 단어: ");
            scanf("%99s", word);
            remove_entry(dict, word);
            break;
        }
        case 3: {
            char word[100];
            printf("검색할 단어: ");
            scanf("%99s", word);
            search_word(dict, word);
            break;
        }
        case 4: {
            char prefix[100];
            printf("자동 완성할 접두사를 입력하세요 (비워두면 모든 단어 출력): ");
            scanf("%99s", prefix);
            list_all_words(dict, prefix);
            break;
        }
        case 5:
            printf("프로그램을 종료합니다.\n");
            return;
        default:
            printf("올바른 옵션을 선택하세요.\n");
            break;
        }
    }
}

int main() {
    Dictionary my_dictionary;
    init_dictionary(&my_dictionary);
    main_menu(&my_dictionary);
    return 0;
}
