#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

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
void parseCode(long long int code, long long int* category, long long int* codeNum, long long int* year, long long int* month, long long int* day) {
    *category = code / 100000000000;          // 10������ ������ õ�� �ڸ��� ����
    *codeNum = (code / 100000000) % 100000;   // �鸸���� ���� �������� õ���� ������ ���� �ڸ��� ����
    *year = (code % 100000000) / 10000;       // 10000���� ������ ������ ����
    *month = (code % 10000) / 100;            // 100���� ������ ���� ����
    *day = code % 100;                        // 100���� ���� �������� ��
}

// �˻� ����� ���Ŀ� �°� ���
void printSearchResult(int resultIndex, BookData book) {
    printf("%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, book.category, book.code, book.year, book.month, book.day);
}


// �˻� �Լ�
void searchBooks(BookData arr[], int dataSize, long long int category, long long int code, long long int year, long long int month, long long int day) {
    bool found = false;
    int resultIndex = 0;

    CLEAR_SCREEN();

    printf("[ �˻� ��� ]\n");

    // �˻��� �����ڵ带 ���
    for (int i = 0; i < dataSize; i++) {
        if ((category == -1 || arr[i].category == category) &&
            (code == -1 || arr[i].code == code) &&
            (year == -1 || arr[i].year == year) &&
            (month == -1 || arr[i].month == month) &&
            (day == -1 || arr[i].day == day)) {
            resultIndex++;
            printSearchResult(resultIndex, arr[i]);
            found = true;
        }
    }

    if (!found) {
        printf("��ġ�ϴ� �����ڵ尡 �����ϴ�.\n");
    }
}

// �˻� �Է��� �޴� �Լ�
long long int getSearchInput(const char* prompt) {
    long long int input;
    while (true) {
        printf("%s", prompt);
        if (scanf("%lld", &input) == 1) {
            break;
        }
        else {
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
            while (getchar() != '\n');  // ���� ����
        }
    }
    return input;
}

// �� ���ڰ� �Է¿� �����ϴ��� Ȯ���ϴ� �Լ�
int isDigitPresent(char* input, char digit) {
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == digit) {
            return 1; // ���ڰ� ������
        }
    }
    return 0; // ���ڰ� �������� ����
}

// �� ���ڿ� ���� ����ڿ��� �˻� ������ �Է� �޴� �Լ�
void promptUserForSearchCriteria(char* input, long long int* searchCategory, long long int* searchCode, long long int* searchYear, long long int* searchMonth, long long int* searchDay) {
    if (isDigitPresent(input, '1')) {
        *searchCategory = getSearchInput("�˻��Ϸ��� �����з� ���� �Է��ϼ��� (-1�� ����): ");
    }
    if (isDigitPresent(input, '2')) {
        *searchCode = getSearchInput("�˻��Ϸ��� �з���ȣ ���� �Է��ϼ��� (-1�� ����): ");
    }
    if (isDigitPresent(input, '3')) {
        *searchYear = getSearchInput("�˻��Ϸ��� �ش� �⵵�� �Է��ϼ��� (-1�� ����): ");
    }
    if (isDigitPresent(input, '4')) {
        *searchMonth = getSearchInput("�˻��Ϸ��� ���� �Է��ϼ��� (-1�� ����): ");
    }
    if (isDigitPresent(input, '5')) {
        *searchDay = getSearchInput("�˻��Ϸ��� �ش����� �Է��ϼ��� (-1�� ����): ");
    }
}

// �ߺ��� ���ڸ� ó���ϰ� ������ ���ڷ� ����� �Լ�
void removeDuplicateDigits(char* input) {
    int len = strlen(input);
    int currentIndex = 1;

    for (int i = 1; i < len; i++) {
        int j;
        for (j = 0; j < currentIndex; j++) {
            if (input[i] == input[j]) {
                break;
            }
        }
        if (j == currentIndex) {
            input[currentIndex] = input[i];
            currentIndex++;
        }
    }

    input[currentIndex] = '\0'; // ���ڿ� ���� �� ���� �߰�
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

    // �˻� ���
    long long int searchCategory = -1;  // -1�� �ش� �׸��� �����ϰ� �˻�
    long long int searchCode = -1;
    long long int searchYear = -1;
    long long int searchMonth = -1;
    long long int searchDay = -1;

    // �˻��� �׸� ����
    printf("�˻��Ϸ��� �׸��� �����ϼ���:\n");
    printf("1. �����з�\n");
    printf("2. �з���ȣ\n");
    printf("3. �⵵\n");
    printf("4. ��\n");
    printf("5. ��\n");

    char input[20];
    int selectedOption;

    while (true) {
        printf("���ڸ� �Է��ϼ���: ");
        scanf("%s", input);

        // �Էµ� �� ���ڸ� Ȯ���Ͽ� ��ȿ�� �˻�
        bool validInput = true;
        for (int i = 0; i < strlen(input); i++) {
            int digit = input[i] - '0';
            if (digit < 1 || digit > 5) {
                validInput = false;
                break;
            }
        }

        if (validInput) {
            // �ùٸ� ���� �ԷµǸ� �ݺ��� ����
            break;
        }
        else {
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
            while (getchar() != '\n');  // ���� ����
        }
    }

    // �Էµ� �� ��� �Ǵ� ������ �� �ֵ��� ���ϴ� ���� ����
    printf("�Էµ� ��: %s\n", input);

    // �ߺ��� ���� ó��
    removeDuplicateDigits(input);

    // �Է¿� ���� ����ڿ��� �˻� ���� �Է� ����
    promptUserForSearchCriteria(input, &searchCategory, &searchCode, &searchYear, &searchMonth, &searchDay);

    // �˻� �Լ� ȣ��
    searchBooks(data, dataSize, searchCategory, searchCode, searchYear, searchMonth, searchDay);

    // �Ҵ�� �޸� ����
    free(data);

    return 0;
}