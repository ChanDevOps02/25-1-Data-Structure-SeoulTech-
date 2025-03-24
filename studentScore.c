#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
struct student { // 학생의 정보를 저장하는 구조체
    int studentID;
    int Korean;
    int English;
    int Math;
    int totScore;
    int avgScore;
};

int main() {
    struct student mystudents[10]; // 10명의 학생 정보를 저장하는 구조체 배열
    int temp_Korean;
    int temp_English;
    int temp_Math;
    int temp_totScore;
    int temp_avgScore;

    for(int i = 0; i < 10; i++){
        mystudents[i].studentID = i + 1; // 학생의 학번을 1부터 10까지 순차적으로 저장
        printf("학생 %d의 국어, 영어, 수학 점수를 입력하세요: ", mystudents[i].studentID);
        scanf("%d %d %d", &temp_Korean, &temp_English, &temp_Math); // 국어, 영어, 수학 점수를 입력받음
        mystudents[i].Korean = temp_Korean; // 입력받은 점수를 구조체에 저장
        mystudents[i].English = temp_English; // 입력받은 점수를 구조체에 저장
        mystudents[i].Math = temp_Math; // 입력받은 점수를 구조체에 저장
        temp_totScore = temp_Korean + temp_English + temp_Math; // 총점 계산
        mystudents[i].totScore = temp_totScore;// 총점을 구조체에 저장
        temp_avgScore = temp_totScore / 3; // 평균 계산
        mystudents[i].avgScore = temp_avgScore; // 평균을 구조체에 저장
    }
    printf("\n");
    printf("학번\t국어\t영어\t수학\t총점\t평균\n");
    printf("*************************************\n");
    for(int i = 0; i < 10; i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", mystudents[i].studentID, mystudents[i].Korean, mystudents[i].English, mystudents[i].Math, mystudents[i].totScore, mystudents[i].avgScore);
    }
    return 0;
}