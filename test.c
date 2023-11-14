#define _CRT_SECURE_NO_WARNINGS()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 퀵 정렬 함수
void quickSort(long long int arr[], int left, int right) {
    int i = left, j = right;
    long long int pivot = arr[(left + right) / 2];

    // 분할
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            // Swap
            long long int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }

    // 재귀적으로 정렬
    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

int main() {
    // 파일 읽기
    FILE* file = fopen("library.csv", "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 1;
    }

    // 데이터 배열 초기 크기
    int initialSize = 30000;

    // 데이터 배열 동적 할당
    long long int* data = malloc(initialSize * sizeof(long long int));
    if (data == NULL) {
        perror("메모리를 할당할 수 없습니다.");
        fclose(file);
        return 1;
    }

    // 데이터 개수와 현재 배열 크기 초기화
    int dataSize = 0;
    int currentSize = initialSize;

    // 파일에서 데이터 읽기
    while (fscanf(file, "%lld", &data[dataSize]) == 1) {
        dataSize++;

        // 배열 크기가 부족하면 두 배로 확장
        if (dataSize == currentSize) {
            currentSize *= 2;
            data = realloc(data, currentSize * sizeof(long long int));
            if (data == NULL) {
                perror("메모리를 재할당할 수 없습니다.");
                fclose(file);
                return 1;
            }
        }
    }

    // 파일 닫기
    fclose(file);

    // 퀵 정렬 수행
    quickSort(data, 0, dataSize - 1);

    // 정렬된 데이터 출력 또는 저장 등을 수행할 수 있음
    // 여기에서는 간단히 출력만 수행
    for (int i = 0; i < dataSize; i++) {
        printf("%lld\n", data[i]);
    }

    // 할당된 메모리 해제
    free(data);

    return 0;
}
