#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
struct Info {
    char name[100];
    int count;
};

int main(void) {
    struct Info studentList[200];
    int count = 5;

    strcpy(studentList[0].name, "다현");  //배열 초기화
    studentList[0].count = 200;

    strcpy(studentList[1].name, "정연");  //배열 초기화
    studentList[1].count = 150;

    strcpy(studentList[2].name, "쯔위");  //배열 초기화
    studentList[2].count = 90;

    strcpy(studentList[3].name, "사나");  //배열 초기화
    studentList[3].count = 30;

    strcpy(studentList[4].name, "지효");  //배열 초기화
    studentList[4].count = 15;

    while (true) {
        printf("[ ");
        for (int i = 0; i < count; i++) {   //배열 전체 출력
            printf("{ ");
            printf("%s %d", studentList[i].name, studentList[i].count);
            printf("} ");
        }
        printf(" ]\n");

        int index = 0; //새로운 친구가 추가되어야 할 배열 내 인덱스
        struct Info newInfo;
        printf("추가할 친구 -->");
        scanf("%s", newInfo.name);
        printf("카톡 횟수 -->");
        scanf("%d", &newInfo.count);

        for (int i = 0; i < count; i++) { //새로운 친구의 정보가 삽입되어야할 배열내 인덱스 알아내기
            if (newInfo.count > studentList[i].count) {
                index = i;
                break;
            }
        }

        for (int i = count - 1; i >= index; i--) {  //뒤에서부터 목표 인덱스까지 배열내의 데이터를 뒤로 밀어내기
            struct Info temp = studentList[i];  //깊은 복사를 통해 임시 변수에 저장
            studentList[i+1] = temp;    //바로 뒤 인덱스에 저장
        }

        studentList[index] = newInfo; //비워진 자리에 추가해야하는 친구의 데이터 저장
        count++;    //인원이 1명씩 증가할 때마다 count 1씩 증가

    }

    return 0;
}