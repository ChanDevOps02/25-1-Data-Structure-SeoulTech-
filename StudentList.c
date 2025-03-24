#include <stdio.h>
#include <stdbool.h>
#include <string.h>
struct address {
    char name[100];
    char phone[100];
    char address[200];
    char birthday[100];
};

int main(void) {
    int n; // 학생 수
    int currentStudent = 0; // 현재 학생 수
    char command[30]; // 명령어
    printf("학생 수 : ");
    scanf("%d", &n);
    struct address students[n];

    while (true) {
        scanf("%s", command);
        if (strcmp(command, "목록보기") == 0) {
            printf("이름\t전화번호\t주소\t생일\n");
            printf("====================================\n");
            for (int i = 0; i < currentStudent; i++) {
                printf("%s\t%s\t%s\t%s\n", students[i].name, students[i].phone, students[i].address, students[i].birthday);
            }
        }
        else if (strcmp(command, "추가하기") == 0) {
            printf("이름 : ");
            scanf("%s", students[currentStudent].name);
            printf("전화번호 : ");
            scanf("%s", students[currentStudent].phone);
            printf("주소 : ");
            scanf("%s", students[currentStudent].address);
            printf("생일 : ");
            scanf("%s", students[currentStudent].birthday);
            currentStudent++;
        }
        else if (strcmp(command, "탐색하기") == 0) {
            int index;// 탐색할 학생의 인덱스
            char tempName[100];
            printf("탐색할 학생의 이름 : ");
            scanf("%s", &tempName);
            for (int i = 0; i < n; i++) {
                if (strcmp(students[i].name, tempName) == 0) {
                    index = i;
                    break;
                }
            }
            printf("이름 : %s\n전화번호 : %s\n주소 : %s\n생일 : %s\n", students[index].name, students[index].phone, students[index].address, students[index].birthday);
        }
        else if (strcmp(command, "삭제하기") == 0) {
            char nameToDelete[100];
            printf("삭제할 학생 이름 : ");
            scanf("%s", nameToDelete);

            for (int i = 0; i < currentStudent; i++) {
                if (strcmp(students[i].name, nameToDelete) == 0) {
                    // 삭제: i 이후 학생들 앞으로 한 칸씩 당기기
                    for (int j = i; j < currentStudent - 1; j++) {
                        students[j] = students[j + 1];
                    }
                    currentStudent--;
                    break;
                }
            }

        }
    }
    return 0;
}