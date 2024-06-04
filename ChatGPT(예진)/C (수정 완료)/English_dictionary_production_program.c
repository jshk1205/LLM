#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 4�� ��� �ܾ� ��� �κп��� ��� ���� �߻�

// Ʈ���� ��� ����ü ����
typedef struct TrieNode {
    struct TrieNode* children[26]; // ���ĺ� �ҹ��� 26��
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

// ���� ����ü ����
typedef struct Dictionary {
    TrieNode* root;
    char* log_file;
} Dictionary;

// ���ο� TrieNode ����
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

// ���� �ʱ�ȭ
void init_dictionary(Dictionary* dict) {
    dict->root = create_trie_node();
    dict->log_file = "dictionary_log.txt";
    // �α� ���Ͽ��� ���� �׸� �ε� ���� (C������ �����ϰ� �����ϱ� �����)
}

// Ʈ���̿� �ܾ� �߰�
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

// Ʈ���̿��� �ܾ� ����
void remove_entry(Dictionary* dict, const char* word) {
    TrieNode* node = dict->root;
    while (*word) {
        int index = *word - 'a';
        if (node->children[index] == NULL) {
            printf("'%s'�� ������ �����ϴ�.\n", word);
            return;
        }
        node = node->children[index];
        word++;
    }
    if (!node->end_of_word) {
        printf("'%s'�� ������ �����ϴ�.\n", word);
        return;
    }
    node->end_of_word = false;
    node->frequency--;
    printf("�������� �����߽��ϴ�.\n", word);
}

// Ʈ���̿��� �ܾ� �˻�
void search_word(Dictionary* dict, const char* word) {
    TrieNode* node = dict->root;
    while (*word) {
        int index = *word - 'a';
        if (node->children[index] == NULL) {
            printf("'%s'�� ���Ǹ� ã�� �� �����ϴ�.\n", word);
            return;
        }
        node = node->children[index];
        word++;
    }
    if (!node->end_of_word) {
        printf("'%s'�� ���Ǹ� ã�� �� �����ϴ�.\n", word);
        return;
    }
    printf("'%s'�� ����: %s\n", word, node->definition);
    printf("����: %s\n", node->pronunciation);
    printf("���Ǿ�: ");
    for (int i = 0; i < node->synonyms_count; i++) {
        printf("%s, ", node->synonyms[i]);
    }
    printf("\n���Ǿ�: ");
    for (int i = 0; i < node->antonyms_count; i++) {
        printf("%s, ", node->antonyms[i]);
    }
    printf("\n����:\n");
    for (int i = 0; i < node->examples_count; i++) {
        printf("- %s\n", node->examples[i]);
    }
    printf("�󵵼�: %d\n", node->frequency);
}

// Ư�� ��忡�� �����Ͽ� ��� �ܾ� ����
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

// ��� �ܾ� ����
void list_all_words(Dictionary* dict, const char* prefix) {
    TrieNode* node = dict->root;
    while (*prefix) {
        int index = *prefix - 'a';
        if (node->children[index] == NULL) {
            printf("������ �ִ� ��� �ܾ�:\n");
            return;
        }
        node = node->children[index];
        prefix++;
    }
    char buffer[100];
    strncpy(buffer, prefix, 100);
    _list_words_from_node(node, buffer, strlen(prefix));
}

// ���� �޴� �Լ�
void main_menu(Dictionary* dict) {
    while (true) {
        printf("\n���� ���α׷�\n");
        printf("1. �ܾ� �߰�\n");
        printf("2. �ܾ� ����\n");
        printf("3. �ܾ� �˻�\n");
        printf("4. ��� �ܾ� ���\n");
        printf("5. ����\n");
        printf("���ϴ� �۾��� �����ϼ���: ");

        int choice;
        scanf("%d", &choice);
        while (getchar() != '\n');  // �Է� ���� ����

        switch (choice) {
        case 1: {
            char word[100], definition[256], pronunciation[100];
            printf("�߰��� �ܾ�: ");
            scanf("%99s", word);
            while (getchar() != '\n');
            printf("�ܾ��� ����: ");
            fgets(definition, 256, stdin);
            definition[strcspn(definition, "\n")] = '\0';
            printf("����: ");
            scanf("%99s", pronunciation);
            while (getchar() != '\n');

            printf("���Ǿ� (��ǥ�� ����): ");
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

            printf("���Ǿ� (��ǥ�� ����): ");
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

            printf("���� (�� ������ ��ǥ�� ����): ");
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
            printf("������ �ܾ�: ");
            scanf("%99s", word);
            remove_entry(dict, word);
            break;
        }
        case 3: {
            char word[100];
            printf("�˻��� �ܾ�: ");
            scanf("%99s", word);
            search_word(dict, word);
            break;
        }
        case 4: {
            char prefix[100];
            printf("�ڵ� �ϼ��� ���λ縦 �Է��ϼ��� (����θ� ��� �ܾ� ���): ");
            scanf("%99s", prefix);
            list_all_words(dict, prefix);
            break;
        }
        case 5:
            printf("���α׷��� �����մϴ�.\n");
            return;
        default:
            printf("�ùٸ� �ɼ��� �����ϼ���.\n");
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
