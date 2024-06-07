//C 코드로 간단한 프로그래밍 언어 인터프리터를 구현해줘
//let 명령어를 사용하여 변수를 선언하고 값을 할당할 수 있으며
//print 명령어를 사용하여 변수의 값을 출력할 수 있어야해
//변수는 정수형만 지원하면 되고, 변수의 이름은 알파벳으로만 이루어져야해
//변수의 이름은 20자 이하로 제한되어야해
//변수의 값은 32비트 정수형으로 제한되어야해
//변수의 값은 음수가 될 수 있어야해
//변수의 값은 10진수로만 입력받아들일 수 있어야해
//변수의 값은 10진수로만 출력할 수 있어야해
//변수의 값은 32비트 정수형 범위를 벗어나면 안돼
//변수의 값은 10진수로 출력할 때 앞에 0을 붙이면 안돼
//변수의 값은 10진수로 출력할 때 뒤에 개행문자를 붙이면 안돼
//변수의 이름은 대소문자를 구분해야해
//변수의 이름은 중복될 수 없어야해

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VARIABLE 100
#define MAX_NAME 20

typedef struct Variable
{
    char name[MAX_NAME];
    int value;
} Variable;

Variable variables[MAX_VARIABLE];
int variableCount = 0;

int findVariable(char* name)
{
    for (int i = 0; i < variableCount; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void let(char* name, int value)
{
    if (findVariable(name) != -1)
    {
        printf("Error: Variable already exists\n");
        return;
    }
    if (strlen(name) > MAX_NAME)
    {
        printf("Error: Variable name is too long\n");
        return;
    }
    if (variableCount >= MAX_VARIABLE)
    {
        printf("Error: Variable count exceeded\n");
        return;
    }
    Variable variable;
    strcpy(variable.name, name);
    variable.value = value;
    variables[variableCount++] = variable;
}

void print(char* name)
{
    int index = findVariable(name);
    if (index == -1)
    {
        printf("Error: Variable not found\n");
        return;
    }
    printf("%d", variables[index].value);
}

int main()
{
    char command[10];
    char name[MAX_NAME];
    int value;
    while (1)
    {
        scanf("%s", command);
        if (strcmp(command, "let") == 0)
        {
            scanf("%s %d", name, &value);
            let(name, value);
        }
        else if (strcmp(command, "print") == 0)
        {
            scanf("%s", name);
            print(name);
        }
    }
    return 0;
}

// Compare this snippet from copilot%28%EB%8F%99%ED%9D%AC%29/C/EncryptionDecryption.c: