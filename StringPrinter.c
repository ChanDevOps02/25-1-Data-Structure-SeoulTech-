#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char* strings[5]; //문자열 포인터 배열 (5개 행)

    for (int i = 0; i < 5; i++) {
        char temp[1000];
        printf("문자열을 입력하세요: ");
        gets(temp);

        strings[i] = (char*)malloc(strlen(temp) + 1); //문자열 길이 + NULL 문자 공간 할당

        strcpy(strings[i], temp); //문자열 복사
    }

    printf("\n입력된 문자열들:\n");
    for (int i = 0; i < 5; i++) {
        printf("Row %d: %s\n", i + 1, strings[i]);
    }
}