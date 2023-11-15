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
        *searchCategory = getSearchInput("�˻��Ϸ��� �����з� ���� �Է��ϼ��� : ");
        while (*searchCategory < 0) {
            printf("�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchCategory = getSearchInput("�˻��Ϸ��� �����з� ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '2')) {
        *searchCode = getSearchInput("�˻��Ϸ��� �з���ȣ ���� �Է��ϼ��� : ");
        while (*searchCode < 0) {
            printf("�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchCode = getSearchInput("�˻��Ϸ��� �з���ȣ ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '3')) {
        *searchYear = getSearchInput("�˻��Ϸ��� �ش� �⵵�� �Է��ϼ��� : ");
        while (*searchYear < 0) {
            printf("�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchYear = getSearchInput("�˻��Ϸ��� �ش� �⵵�� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '4')) {
        *searchMonth = getSearchInput("�˻��Ϸ��� ���� �Է��ϼ��� : ");
        while (*searchMonth < 0) {
            printf("�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchMonth = getSearchInput("�˻��Ϸ��� ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '5')) {
        *searchDay = getSearchInput("�˻��Ϸ��� �ش����� �Է��ϼ��� : ");
        while (*searchDay < 0) {
            printf("�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchDay = getSearchInput("�˻��Ϸ��� �ش����� �Է��ϼ��� : ");
        }
    }
}

// �˻� �Լ�
void searchBooks(BookData arr[], int dataSize) {
    long long int searchCategory;
    long long int searchCode;
    long long int searchYear;
    long long int searchMonth;
    long long int searchDay;

    printf("�˻��Ϸ��� �׸��� �����ϼ���:\n");
    printf("1. �����з�\n");
    printf("2. �з���ȣ\n");
    printf("3. �⵵\n");
    printf("4. ��\n");
    printf("5. ��\n");

    char input[20];

    // ����� �Է� �ޱ�
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

    // ���� �˻� ����
    bool found = false;
    int resultIndex = 0;

    printf("[ �˻� ��� ]\n");

    // �˻��� �����ڵ带 ���
    for (int i = 0; i < dataSize; i++) {
        if (
            (searchCategory == arr[i].category || searchCategory < 0) &&
            (searchCode == arr[i].code || searchCode < 0) &&
            (searchYear == arr[i].year || searchYear < 0) &&
            (searchMonth == arr[i].month || searchMonth < 0) &&
            (searchDay == arr[i].day || searchDay < 0)
            ) {
            resultIndex++;
            printf("%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, arr[i].category, arr[i].code, arr[i].year, arr[i].month, arr[i].day);
            found = true;
        }
    }

    if (!found) {
        printf("��ġ�ϴ� �����ڵ尡 �����ϴ�.\n");
    }
}

// ������ ������ �迭�� �߰��ϴ� �Լ�
void addBook(BookData arr[], int* dataSize, int* currentSize) {
    if (*dataSize == *currentSize) {
        // �迭�� �� á�� ��� ũ�⸦ �� ��� �ø��ϴ�.
        *currentSize *= 2;
        arr = realloc(arr, *currentSize * sizeof(BookData));
        if (arr == NULL) {
            perror("�޸𸮸� ���Ҵ��� �� �����ϴ�.");
            exit(1);
        }
    }

    // ���ο� ������ ���� �Է��� �޽��ϴ�.
    printf("�����з� �Է�: ");
    scanf("%lld", &arr[*dataSize].category);
    printf("�з���ȣ �Է�: ");
    scanf("%lld", &arr[*dataSize].code);
    printf("�⵵ �Է�: ");
    scanf("%lld", &arr[*dataSize].year);
    printf("�� �Է�: ");
    scanf("%lld", &arr[*dataSize].month);
    printf("�� �Է�: ");
    scanf("%lld", &arr[*dataSize].day);

    (*dataSize)++;
}

// ������ ������ �迭���� �����ϴ� �Լ�
void modifyBook(BookData arr[], int dataSize) {
    long long int searchCode;
    printf("�����Ϸ��� ������ �����ڵ带 �Է��ϼ���: ");
    scanf("%lld", &searchCode);

    int foundIndex = -1;
    for (int i = 0; i < dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        // ���� ������ �Է¹޽��ϴ�.
        printf("�����з� ����: ");
        scanf("%lld", &arr[foundIndex].category);
        printf("�з���ȣ ����: ");
        scanf("%lld", &arr[foundIndex].code);
        printf("�⵵ ����: ");
        scanf("%lld", &arr[foundIndex].year);
        printf("�� ����: ");
        scanf("%lld", &arr[foundIndex].month);
        printf("�� ����: ");
        scanf("%lld", &arr[foundIndex].day);

        printf("���� ������ �����Ǿ����ϴ�.\n");
    }
    else {
        printf("��ġ�ϴ� �����ڵ尡 �����ϴ�.\n");
    }
}

// ������ ������ �迭���� �����ϴ� �Լ�
void deleteBook(BookData arr[], int* dataSize) {
    long long int searchCode;
    printf("�����Ϸ��� ������ �����ڵ带 �Է��ϼ���: ");
    scanf("%lld", &searchCode);

    int foundIndex = -1;
    for (int i = 0; i < *dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        // ������ ������ ������ ��ġ�� �̵��ϰ� dataSize�� ���ҽ�ŵ�ϴ�.
        arr[foundIndex] = arr[*dataSize - 1];
        (*dataSize)--;
        printf("������ �����Ǿ����ϴ�.\n");
    }
    else {
        printf("��ġ�ϴ� �����ڵ尡 �����ϴ�.\n");
    }
}

// ���� �޴� �Լ�
void showMainMenu() {
    printf("���� ���� ���α׷�\n");
    printf("1. �����ڵ� �˻�\n");
    printf("2. �����ڵ� �߰�\n");
    printf("3. �����ڵ� ����\n");
    printf("4. �����ڵ� ����\n");
    printf("0. ���α׷� ����\n");
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

    int choice = -1;

    do {
        showMainMenu();
        printf("���ϴ� �۾��� �����ϼ���: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // �����ڵ� �˻�
            searchBooks(data, dataSize);
            break;
        case 2:
            // �����ڵ� �߰�
            addBook(data, &dataSize, &currentSize);
            break;
        case 3:
            // �����ڵ� ����
            modifyBook(data, dataSize);
            break;
        case 4:
            // �����ڵ� ����
            deleteBook(data, &dataSize);
            break;
        case 0:
            // ���α׷� ����
            printf("���α׷��� �����մϴ�.\n");
            break;
        default:
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
            break;
        }
    } while (choice != 0); // 0�� �����ϸ� ������ Ż���մϴ�.

    // �Ҵ�� �޸� ����
    free(data);

    return 0;
}