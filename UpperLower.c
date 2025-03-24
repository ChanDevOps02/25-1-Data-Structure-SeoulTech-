#include <stdio.h>
#include <ctype.h>

int main(void) {
    char str[100];
    char* ptr = str;

    fgets(str, sizeof(str), stdin);
    while (*ptr != '\0') {
        if (islower(*ptr)) {
            *ptr = toupper(*ptr);
        }
        else if (isupper(*ptr)) {
            *ptr = tolower(*ptr);
        }
        ptr++;
    }
    printf("변환된 문자열 : %s\n", str);
    return 0;
}