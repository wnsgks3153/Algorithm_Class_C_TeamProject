#define _CRT_SECURE_NO_WARNINGS
// �������� ǥ���� ������ ��
#define BOOKS_PER_PAGE 8000

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>



typedef struct {
    long long int code;       // �����ڵ���ü(14�ڸ�)
    long long int category;   // �����з�
    long long int group;      // �з���ȣ
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
        while (arr[i].category < pivot.category || (arr[i].category == pivot.category && arr[i].group < pivot.group) || (arr[i].category == pivot.category && arr[i].group == pivot.group && (arr[i].year < pivot.year || (arr[i].year == pivot.year && (arr[i].month < pivot.month || (arr[i].month == pivot.month && arr[i].day < pivot.day))))))
            i++;
        while (arr[j].category > pivot.category || (arr[j].category == pivot.category && arr[j].group > pivot.group) || (arr[j].category == pivot.category && arr[j].group == pivot.group && (arr[j].year > pivot.year || (arr[j].year == pivot.year && (arr[j].month > pivot.month || (arr[j].month == pivot.month && arr[j].day > pivot.day))))))
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
void parseCode(long long int code, long long int* category, long long int* group, long long int* year, long long int* month, long long int* day) {
    *category = code / 100000000000;          // 10������ ������ õ�� �ڸ��� ����
    *group = (code / 100000000) % 100000;   // �鸸���� ���� �������� õ���� ������ ���� �ڸ��� ����
    *year = (code % 100000000) / 10000;       // 10000���� ������ ������ ����
    *month = (code % 10000) / 100;            // 100���� ������ ���� ����
    *day = code % 100;                        // 100���� ���� �������� ��
}


// �˻� ����� ���Ŀ� �°� ���
void printSearchResult(int resultIndex, BookData book) {
    printf("%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, book.category, book.group, book.year, book.month, book.day);
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
void promptUserForSearchCriteria(char* input, long long int* searchCategory, long long int* searchGroup, long long int* searchYear, long long int* searchMonth, long long int* searchDay) {
    if (isDigitPresent(input, '1')) {
        *searchCategory = getSearchInput("�˻��Ϸ��� �����з� ���� �Է��ϼ��� : ");
        while (*searchCategory < 0) {
            printf("�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchCategory = getSearchInput("�˻��Ϸ��� �����з� ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '2')) {
        *searchGroup = getSearchInput("�˻��Ϸ��� �з���ȣ ���� �Է��ϼ��� : ");
        while (*searchGroup < 0) {
            printf("�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchGroup = getSearchInput("�˻��Ϸ��� �з���ȣ ���� �Է��ϼ��� : ");
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

// ������ ��� �Լ�
void printPage(BookData arr[], int dataSize, int startIdx, int endIdx) {
    for (int i = startIdx; i <= endIdx; i++) {
        printf("%d. %03lld%03lld%04lld%02lld%02lld\n", i + 1, arr[i].category, arr[i].group, arr[i].year, arr[i].month, arr[i].day);
    }
}

// ��ü ���� �ڵ� ��� �Լ�
void printAllBooks(BookData arr[], int dataSize) {
    CLEAR_SCREEN();

    int totalPages = (dataSize + BOOKS_PER_PAGE - 1) / BOOKS_PER_PAGE; // �ø��� �̿��Ͽ� ������ �� ���

    int currentPage = 1;
    int startIdx = 0;
    int endIdx = (dataSize < BOOKS_PER_PAGE) ? dataSize - 1 : BOOKS_PER_PAGE - 1;

    while (true) {
        printPage(arr, dataSize, startIdx, endIdx);

        // ���� ������ �Ǵ� ���� �������� �̵����� ���� ����
        char moveKey;
        if (currentPage == totalPages) {
            // ������ �������� ��� ���� �������θ� �̵� ����
            printf("[ �����ڵ� ��ü ����Ʈ (������ %d / %d) ]\n", currentPage, totalPages);
            printf("\n���� �������� �̵�[p] ����[q]\n");
            printf("����: ");
            scanf(" %c", &moveKey);
            if (moveKey == 'p' && currentPage > 1) {
                // ���� �������� �̵�
                startIdx -= BOOKS_PER_PAGE;
                endIdx = startIdx + BOOKS_PER_PAGE - 1;
                currentPage--;
                CLEAR_SCREEN();
            }
            else if (moveKey == 'q') {
                break;
            }
        }
        else {
            // �� ���� ������������ ������������ ���������� ���� ����
            printf("[ �����ڵ� ��ü ����Ʈ (������ %d / %d) ]\n", currentPage, totalPages);
            printf("\n���� �������� �̵�[n] ���� �������� �̵�[p] ����[q]\n");
            printf("����: ");
            char moveKey;
            scanf(" %c", &moveKey);
            if (moveKey == 'p' && currentPage > 1) {
                // ���� �������� �̵�
                startIdx -= BOOKS_PER_PAGE;
                endIdx = startIdx + BOOKS_PER_PAGE - 1;
                currentPage--;
                CLEAR_SCREEN();
            }
            else if (moveKey == 'n' && currentPage < totalPages) {
                // ���� �������� �̵�
                startIdx += BOOKS_PER_PAGE;
                endIdx = (endIdx + BOOKS_PER_PAGE < dataSize - 1) ? endIdx + BOOKS_PER_PAGE : dataSize - 1;
                currentPage++;
                CLEAR_SCREEN();
            }
            else if (moveKey == 'q') {
                break;
            }
        }
    }
}

// �˻� �Լ�
void searchBooks(BookData arr[], int dataSize) {

    CLEAR_SCREEN();

    long long int searchCategory;
    long long int searchGroup;
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
    promptUserForSearchCriteria(input, &searchCategory, &searchGroup, &searchYear, &searchMonth, &searchDay);

    // ���� �˻� ����
    bool found = false;
    int resultIndex = 0;

    CLEAR_SCREEN();
    printf("[ �˻� ��� ]\n");

    // �˻��� �����ڵ带 ���
    for (int i = 0; i < dataSize; i++) {
        if (
            (searchCategory == arr[i].category || searchCategory < 0) &&
            (searchGroup == arr[i].group || searchGroup < 0) &&
            (searchYear == arr[i].year || searchYear < 0) &&
            (searchMonth == arr[i].month || searchMonth < 0) &&
            (searchDay == arr[i].day || searchDay < 0)
            ) {
            resultIndex++;
            printf("%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, arr[i].category, arr[i].group, arr[i].year, arr[i].month, arr[i].day);
            found = true;
        }
    }

    if (!found) {
        printf("��ġ�ϴ� �����ڵ尡 �����ϴ�.\n");
    }

}

// ���Ͽ� ���� �ڵ带 �߰��ϴ� �Լ�
void writeBookToFile(FILE* file, BookData book) {
    fprintf(file, "%03lld%03lld%04lld%02lld%02lld\n", book.category, book.group, book.year, book.month, book.day);
}

// ���������� ���� ���ϴ� ��ü �ڵ� ����
void generateReplacementCode(char* replacementCode) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // 'NSC(���� ��¥)' ���·� ��ü �ڵ� ����
    sprintf(replacementCode, "NSC%d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// ���� �ڵ��� ��ȿ���� Ȯ���ϴ� �Լ�
bool isValidCode(const char* input) {
    // ���̰� 14�ڸ����� Ȯ��
    if (strlen(input) != 14) {
        return false;
    }

    // ��� ���ڰ� �������� Ȯ��
    for (int i = 0; i < 14; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return false;
        }
    }

    return true;
}

// ��¥�� ��ȿ���� Ȯ���ϴ� �Լ�
bool isValidDate(long long int year, long long int month, long long int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    // �� ���� �ϼ��� Ȯ���Ͽ� ��ȿ�� �˻�
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        // ������ ��� 2���� 29�ϱ��� ��ȿ
        daysInMonth[2] = 29;
    }

    return day <= daysInMonth[month];
}

// ���ο� ������ ������ �迭�� �߰��ϴ� �Լ�
void addBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    char inputCode[15];  // �ڵ带 �Է¹��� ���ڿ�, �ڵ�� 14�ڸ��̸� �� ���� ���ڸ� ����� 15�� ����
    long long int code;

    // �ܰ� 1: ���� �ڵ� �Է� (���ڿ���)
    while (true) {
        printf("�����ڵ带 �Է��ϼ���: ");
        scanf("%s", inputCode);

        // �ܰ� 2: �Է��� ���ڿ� �Ǵ� ���� �������� Ȯ��
        if (isValidCode(inputCode)) {
            // ���ڿ��� long long int�� ��ȯ
            code = atoll(inputCode);

            // �ܰ� 3: �ڵ忡 ��ȿ�� ��¥�� �ִ��� Ȯ��
            long long int category, group, year, month, day;
            parseCode(code, &category, &group, &year, &month, &day);

            // ��¥�� ��ȿ���� ���� ��� �ٽ� �Է� ����
            if (!isValidDate(year, month, day)) {
                CLEAR_SCREEN();
                printf("��ȿ���� ���� ��¥�Դϴ�. �ٽ� �Է��ϼ���.\n");
                continue;
            }

            // �ڵ尡 ��ȿ�� ���, å�� ���Ͽ� �߰�
            FILE* file = fopen("library.csv", "a");
            if (file == NULL) {
                CLEAR_SCREEN();
                perror("������ �� �� �����ϴ�.");
                return;
            }

            // ���Ͽ� ������ �ۼ�
            writeBookToFile(file, (BookData) { code, category, group, year, month, day });

            // ���� �ݱ�
            fclose(file);

            printf("������ ���������� �߰��Ǿ����ϴ�.\n");
            break;
        }
        else {
            // ���������� �ڵ��� ��� ��ü �ڵ带 �����Ͽ� ���Ͽ� �߰�
            printf("�߸��� �����ڵ� �����Դϴ�. ��ü �ڵ带 �����Ͽ� �߰��մϴ�.\n");

            char replacementCode[15];
            generateReplacementCode(replacementCode);

            FILE* file = fopen("library.csv", "a");
            if (file == NULL) {
                CLEAR_SCREEN();
                perror("������ �� �� �����ϴ�.");
                return;
            }

            // ��ü �ڵ带 ���Ͽ� �ۼ�
            fprintf(file, "%s\n", replacementCode);

            // ���� �ݱ�
            fclose(file);

            printf("�߰��� ��ü�ڵ� : %s\n", replacementCode);
            break;
        }
    }
}

// ������ ������ �迭���� �����ϴ� �Լ�
void modifyBook(BookData arr[], int dataSize) {
    CLEAR_SCREEN();
    long long int searchCode;
    long long int code;

    // 1. �� �Է�
    printf("������ �����ڵ带 �Է��ϼ���: ");
    scanf("%lld", &searchCode);

    // 2. �����ڵ� Ž��
    int foundIndex = -1;
    for (int i = 0; i < dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    // 3. �����ڵ尡 ���� ���
    if (foundIndex == -1) {
        printf("�ش� �����ڵ尡 �������� �ʽ��ϴ�.\n");

        char retry;
        printf("�ٽ� �Է��Ͻðڽ��ϱ�? [y/n]: ");
        scanf(" %c", &retry);

        // 4. �ٽ� �Է��� ������ Ȯ��
        if (retry == 'y') {
            modifyBook(arr, dataSize); // ��� ȣ���� ���� �Լ� �����
        }
        else {
            printf("���α׷��� �����մϴ�.\n");
            return; // ����
        }
    }
    else {
        char confirmModify;
        // 5. ���� ���� �ٽ� Ȯ��
        printf("�����ڵ� %lld�� �����Ͻðڽ��ϱ�? [y/n]: ", searchCode);
        scanf(" %c", &confirmModify);

        if (confirmModify == 'y') {
            // 6. ������ ���� �Է¹���
            char newCode[15];
            do {
                printf("���ο� �����ڵ带 �Է��ϼ���: ");
                scanf("%s", newCode);

                // 7. �Էµ� ���� ��ȿ���� Ȯ��
                if (!isValidCode(newCode)) {
                    printf("��ȿ���� ���� �����ڵ��Դϴ�. �ٽ� �Է��ϼ���.\n");
                }
                else {
                    // 8. ��ȿ�� ���, ��¥�� �����ϴ��� Ȯ��
                    code = atoll(newCode);
                    long long int category, group, year, month, day;
                    parseCode(code, &category, &group, &year, &month, &day);

                    if (!isValidDate(year, month, day)) {
                        printf("��ȿ���� ���� ��¥�Դϴ�. �ٽ� �Է��ϼ���.\n");
                    }
                    else {
                        // 9. ��¥�� ��ȿ�� ���, �����ڵ带 ����
                        arr[foundIndex].code = code;
                        printf("�����ڵ尡 ���������� �����Ǿ����ϴ�.\n");

                        // 10. ���� ��� ������ �ڵ�� ������
                        FILE* file = fopen("library.csv", "r");
                        if (file == NULL) {
                            perror("������ �� �� �����ϴ�.");
                            return;
                        }

                        // �ӽ� ���� ����
                        FILE* tempFile = fopen("temp.csv", "w");
                        if (tempFile == NULL) {
                            perror("�ӽ� ������ ������ �� �����ϴ�.");
                            fclose(file);
                            return;
                        }

                        // ���� ������ �о ������ �ڵ�� ������
                        long long int currentCode;
                        while (fscanf(file, "%lld", &currentCode) == 1) {
                            if (currentCode == searchCode) {
                                // ������ �ڵ�� ��ü
                                fprintf(tempFile, "%lld\n", code);
                            }
                            else {
                                // ���� �ڵ� �״�� ����
                                fprintf(tempFile, "%lld\n", currentCode);
                            }
                        }

                        // ���� �ݱ�
                        fclose(file);
                        fclose(tempFile);

                        // ������ �Ϸ�� �ӽ� ������ ���� ���Ϸ� ����
                        remove("library.csv"); // ���� ���� ����
                        rename("temp.csv", "library.csv"); // �ӽ� ������ ���� ���Ϸ� �̸� ����

                        break;
                    }
                }
            } while (true);
        }
        else {
            printf("�����ڵ� ������ ��ҵǾ����ϴ�.\n");
        }
    }
}

// ������ ������ �迭���� �����ϴ� �Լ�
void deleteBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    long long int searchCode;

    // 1. �� �Է�
    printf("������ �����ڵ带 �Է��ϼ���: ");
    scanf("%lld", &searchCode);

    // 2. �����ڵ� Ž��
    int foundIndex = -1;
    for (int i = 0; i < *dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    // 3. �����ڵ尡 ���� ���
    if (foundIndex == -1) {
        printf("�ش� �����ڵ尡 �������� �ʽ��ϴ�.\n");

        char retry;
        printf("�ٽ� �Է��Ͻðڽ��ϱ�? [y/n]: ");
        scanf(" %c", &retry);

        // 4. �ٽ� �Է��� ������ Ȯ��
        if (retry == 'y') {
            deleteBook(arr, dataSize); // ��� ȣ���� ���� �Լ� �����
        }
        else {
            printf("���α׷��� �����մϴ�.\n");
            return; // ����
        }
    }
    else {
        char confirmDelete;
        // 5. ���� ���� �ٽ� Ȯ��
        printf("�����ڵ� %lld�� �����Ͻðڽ��ϱ�? [y/n]: ", searchCode);
        scanf(" %c", &confirmDelete);

        if (confirmDelete == 'y') {
            // 6. ���� ����
            for (int i = foundIndex; i < *dataSize - 1; i++) {
                arr[i] = arr[i + 1];
            }

            (*dataSize)--;
            printf("�����ڵ尡 ���������� �����Ǿ����ϴ�.\n");
        }
        else {
            printf("�����ڵ� ������ ��ҵǾ����ϴ�.\n");
        }
    }
}

// ���� �޴� �Լ�
void showMainMenu() {
    printf("���� ���� ���α׷�\n");
    printf("1. �����ڵ� ��ü ����Ʈ ���\n");
    printf("2. �����ڵ� �˻�\n");
    printf("3. �����ڵ� �߰�\n");
    printf("4. �����ڵ� ����\n");
    printf("5. �����ڵ� ����\n");
    printf("0. ���α׷� ����\n");
}

int runMainMenu(BookData arr[], int dataSize) {
    int choice = -1;

    do {
        showMainMenu();
        printf("���ϴ� �۾��� �����ϼ���: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // �����ڵ� ��ü ����Ʈ ���
            printAllBooks(arr, dataSize);
            break;
        case 2:
            // �����ڵ� �˻�
            searchBooks(arr, dataSize);
            break;
        case 3:
            // �����ڵ� �߰�
            addBook(arr, &dataSize); // arr�� dataSize�� �Լ��� ����
            break;
        case 4:
            // �����ڵ� ����
            modifyBook(arr, dataSize);
            break;
        case 5:
            // �����ڵ� ����
            deleteBook(arr, &dataSize); // arr�� dataSize�� �Լ��� ����
            break;
        case 0:
            // ���α׷� ����
            printf("���α׷��� �����մϴ�.\n");
            break;
        default:
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
            break;
        }

        if (choice != 0) {
            // ����ڿ��� ���� �޴��� ���ư��ڳĴ� ���θ� ����ϴ�.
            char returnToMain;
            printf("���� ȭ������ ���ư��ðڽ��ϱ�? [y/n] : ");
            scanf(" %c", &returnToMain);

            if (returnToMain != 'y') {
                choice = 0; // ����ڰ� ���ư��⸦ ������ ������ ������ �����Ͽ� ���α׷� ����
            }
            else {
                // ������ 'y'�� ���, ���� ȭ������ ���ư��� ���� ȭ���� ���쵵�� ����
                CLEAR_SCREEN();
            }
        }

    } while (choice != 0); // 0�� ���α׷��� �����մϴ�.

    // ������� ���������� ��Ÿ���� ���� ��ȯ
    return choice;
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
        parseCode(data[dataSize].code, &data[dataSize].category, &data[dataSize].group, &data[dataSize].year, &data[dataSize].month, &data[dataSize].day);
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

    int continueProgram;

    do {
        continueProgram = runMainMenu(data, dataSize);
    } while (continueProgram != 0);
    
    // �Ҵ�� �޸� ����
    free(data);
    CLEAR_SCREEN();
    printf("���α׷��� ����Ǿ����ϴ�.");
    return 0;
}