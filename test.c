#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long int category;   // �����з�
    long long int code;       // �з���ȣ
    long long int year;       // ����
    long long int month;      // ��
    long long int day;        // ��
} BookData;

// �� ���� �Լ�
void swap(BookData* a, BookData* b) {
    BookData temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(BookData arr[], int left, int right) {
    int i = left, j = right;
    BookData pivot = arr[(left + right) / 2];

    // ����
    while (i <= j) {
        while (arr[i].category < pivot.category || (arr[i].category == pivot.category && arr[i].code < pivot.code) || (arr[i].category == pivot.category && arr[i].code == pivot.code && (arr[i].year < pivot.year || (arr[i].year == pivot.year && (arr[i].month < pivot.month || (arr[i].month == pivot.month && arr[i].day < pivot.day))))))
            i++;
        while (arr[j].category > pivot.category || (arr[j].category == pivot.category && arr[j].code > pivot.code) || (arr[j].category == pivot.category && arr[j].code == pivot.code && (arr[j].year > pivot.year || (arr[j].year == pivot.year && (arr[j].month > pivot.month || (arr[j].month == pivot.month && arr[j].day > pivot.day))))))
            j--;
        if (i <= j) {
            // Swap ȣ��
            swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

// �� �ڸ����� �и��Ͽ� ����ü ����� ����
void parseCode(long long int code, int* category, int* codeNum, int* year, int* month, int* day) {
    *category = (int)(code / 100000000000);          // 10������ ������ õ�� �ڸ��� ����
    *codeNum = (int)((code / 100000000) % 100000);     // �鸸���� ���� �������� õ���� ������ ���� �ڸ��� ����
    *year = (int)((code % 100000000) / 10000);       // 10000���� ������ ������ ����
    *month = (int)((code % 10000) / 100);          // 100���� ������ ���� ����
    *day = (int)(code % 100);                      // 100���� ���� �������� ��
}

int main() {
    // ���� �б�
    FILE* file = fopen("library.csv", "r");
    if (file == NULL) {
        perror("������ �� �� �����ϴ�.");
        return 1;
    }

    // ������ �迭 �ʱ� ũ��
    int initialSize = 30000;

    // ������ �迭 ���� �Ҵ�
    BookData* data = malloc(initialSize * sizeof(BookData));
    if (data == NULL) {
        perror("�޸𸮸� �Ҵ��� �� �����ϴ�.");
        fclose(file);
        return 1;
    }

    // ������ ������ ���� �迭 ũ�� �ʱ�ȭ
    int dataSize = 0;
    int currentSize = initialSize;

    // ���Ͽ��� ������ �б�
    while (fscanf(file, "%lld", &data[dataSize].code) == 1) {
        parseCode(data[dataSize].code, &data[dataSize].category, &data[dataSize].code, &data[dataSize].year, &data[dataSize].month, &data[dataSize].day);
        dataSize++;

        // �迭 ũ�Ⱑ �����ϸ� �� ��� Ȯ��
        if (dataSize == currentSize) {
            currentSize *= 2;
            data = realloc(data, currentSize * sizeof(BookData));
            if (data == NULL) {
                perror("�޸𸮸� ���Ҵ��� �� �����ϴ�.");
                fclose(file);
                return 1;
            }
        }
    }

    // ���� �ݱ�
    fclose(file);

    // �� ���� ����
    quickSort(data, 0, dataSize - 1);

    // ���ĵ� ������ ���
    for (int i = 0; i < dataSize; i++) {
        printf("�����з�: %03d, �з���ȣ: %03d, ��: %04d, ��: %02d, ��: %02d\n", data[i].category, data[i].code, data[i].year, data[i].month, data[i].day);
    }

    // �Ҵ�� �޸� ����
    free(data);

    return 0;
}
