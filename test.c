#define _CRT_SECURE_NO_WARNINGS()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �� ���� �Լ�
void quickSort(long long int arr[], int left, int right) {
    int i = left, j = right;
    long long int pivot = arr[(left + right) / 2];

    // ����
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

    // ��������� ����
    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
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
    long long int* data = malloc(initialSize * sizeof(long long int));
    if (data == NULL) {
        perror("�޸𸮸� �Ҵ��� �� �����ϴ�.");
        fclose(file);
        return 1;
    }

    // ������ ������ ���� �迭 ũ�� �ʱ�ȭ
    int dataSize = 0;
    int currentSize = initialSize;

    // ���Ͽ��� ������ �б�
    while (fscanf(file, "%lld", &data[dataSize]) == 1) {
        dataSize++;

        // �迭 ũ�Ⱑ �����ϸ� �� ��� Ȯ��
        if (dataSize == currentSize) {
            currentSize *= 2;
            data = realloc(data, currentSize * sizeof(long long int));
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

    // ���ĵ� ������ ��� �Ǵ� ���� ���� ������ �� ����
    // ���⿡���� ������ ��¸� ����
    for (int i = 0; i < dataSize; i++) {
        printf("%lld\n", data[i]);
    }

    // �Ҵ�� �޸� ����
    free(data);

    return 0;
}
