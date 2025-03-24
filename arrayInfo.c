#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
void printArray(int array[]) {
    for (int i = 0; i < 6; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
void minArray(int array[]) {
    int min = array[0];
    for (int i = 1; i < 6; i++) {
        if (min > array[i]) {
            min = array[i];
        }
    }
    printf("최소값: %d\n", min);
}
void maxArray(int array[]) {
    int max = array[0];
    for (int i = 1; i < 6; i++) {
        if (max < array[i]) {
            max = array[i];
        }
    }
    printf("최대값: %d\n", max);
}
void avgArray(int array[]) {
    int sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += array[i];
    }
    printf("평균값: %.2f\n", (float)sum / 6);
}
void varArray(int array[]) {
    float avg = 0;
    float sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += array[i];
    }
    avg = sum / 6;
    sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += (array[i] - avg) * (array[i] - avg);
    }
    printf("분산값: %.2f\n", sum / 6);
}

int main(void) {
    int array[6] = {19, 2, 25, 92, 36, 45};
    printArray(array); //배열을 출력하는 함수
    minArray(array); //배열의 최소값을 출력하는 함수
    maxArray(array); //배열의 최대값을 출력하는 함수
    avgArray(array); //배열의 평균값을 출력하는 함수
    varArray(array); //배열의 분산값을 출력하는 함수

    return 0;
}