#include <stdio.h>
#include <ctype.h>

int main(void) {
    char str[100]; //문자열을 저장할 공간
    char* ptr = str; //배열의 이름은 배열의 첫번재 공간의 주소이므로 이를 char 포인터형에 저장

    fgets(str, sizeof(str), stdin); //문자열 입력받기
    while (*ptr != '\0') {//해당 포인터가 가리키는 문자가 공백일 때까지 반복
        if (islower(*ptr)) { //소문자라면
            *ptr = toupper(*ptr); //대문자로 변환
        }
        else if (isupper(*ptr)) { //대문자라면
            *ptr = tolower(*ptr); //소문자로 변환
        }
        ptr++; //포인터값 1씩 증가
    }
    printf("변환된 문자열 : %s\n", str);
    return 0;
}
