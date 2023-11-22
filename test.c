#define _CRT_SECURE_NO_WARNINGS
// �������� ǥ���� ������ ��
#define BOOKS_PER_PAGE 8000

//��� ������Ʈ ȭ�� ����� ��ɾ� ����
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
#include <windows.h>

// �����ڵ� �׸񺰷� ������ ����ü ����
typedef struct {
    long long int code;       // �����ڵ���ü(14�ڸ�)
    long long int category;   // �����з�
    long long int group;      // �з���ȣ
    long long int year;       // ����
    long long int month;      // ��
    long long int day;        // ��
} BookData;

// ��� �� �߾ӿ��� ����ϴ� ��ɾ� ����
void centerText(const char* text) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int padding = (width - strlen(text)) / 2;
    for (int i = 0; i < padding; i++) {
        putchar(' ');
    }

    printf("%s", text);
}

// �� ���� �Լ����� ����ϴ� SWAP �Լ�
void swap(BookData* a, BookData* b) {
    BookData temp = *a;
    *a = *b;
    *b = temp;
}

// �����ڵ� �����͸� �ҷ��ͼ� �����͸� ���������� �����ϱ� ���� ������ �Լ�
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

// �����ڵ��� �� �ڸ����� �и��Ͽ� ����ü ����� ����
void parseCode(long long int code, long long int* category, long long int* group, long long int* year, long long int* month, long long int* day) {
    *category = code / 100000000000;          // �����ڵ带 1000������ ������ 14�ڸ� �����ڵ��� �տ��� 1~3��° ��ġ�� �ڵ带 �����з� �׸� ����
    *group = (code / 100000000) % 100000;     // �����ڵ带 1������ ���� ���� 10������ ������ 14�ڸ� �����ڵ��� �տ��� 1~6��° ��ġ�� �ڵ带 �з���ȣ �׸� ����
    *year = (code % 100000000) / 10000;       // �����ڵ带 1������ ���� �������� 1������ ������ 14�ڸ� �����ڵ��� �տ��� 7~10��° ��ġ�� �ڵ带 �⵵ �׸� ����
    *month = (code % 10000) / 100;            // �����ڵ带 1������ ���� �������� ������ ������ 14�ڸ� �����ڵ��� �տ��� 11~12��° ��ġ�� �ڵ带 �⵵ �׸� ����
    *day = code % 100;                        // �����ڵ带 100���� ���� �������� 14�ڸ� �����ڵ��� �տ��� 13~14��° ��ġ�� �ڵ带 �⵵ �׸� ����
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

// �����ڵ� �˻� �Լ����� �׸� ���� �˻� �Է��� �޴� �Լ�
long long int getSearchInput(const char* prompt) {
    long long int input;
    while (true) { // �ùٸ� ���� �Էµ� ������ �ݺ�
        printf("%s", prompt);
        if (scanf("%lld", &input) == 1) {
            break; // �ùٸ� ���� �ԷµǾ��ٸ� �Լ��� ����
        }
        else {
            printf("\t\t\t\t\t  �߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n"); //
            while (getchar() != '\n');  // �߸��� ���� �Էµ� ��� ����� ����ϰ� �Է� �޾Ҵ� ���� ���۸� �ʱ�ȭ
        }
    }
    return input; // �Էµ� ���� �ٽ� ��ȯ
}

// �� ���ڰ� �Է¿� �����ϴ��� Ȯ���ϴ� �Լ�
int isDigitPresent(char* input, char digit) {
    for (int i = 0; i < strlen(input); i++) { // �ѱ��ھ� ���� �Է��� �����ϴ��� Ȯ��
        if (input[i] == digit) {
            return 1; // ���ڰ� ������
        }
    }
    return 0; // ���ڰ� �������� ����
}

// �� ���ڿ� ���� ����ڿ��� �˻� ������ �Է� �޴� �Լ�
void promptUserForSearchCriteria(char* input, long long int* searchCategory, long long int* searchGroup, long long int* searchYear, long long int* searchMonth, long long int* searchDay) {
    if (isDigitPresent(input, '1')) { // 1�� ������ ��� �˻��� �����з� ���� �Է� ����
        *searchCategory = getSearchInput("\t\t\t\t\t�˻��� ���� �з� ���� �Է��ϼ��� : ");
        while (*searchCategory < 0) {
            printf("\t\t\t\t\t�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchCategory = getSearchInput("\t\t\t\t\t�˻��� ���� �з� ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '2')) { // 2�� ������ ��� �˻��� �з���ȣ ���� �Է� ����
        *searchGroup = getSearchInput("\t\t\t\t\t�˻��� �з� ��ȣ ���� �Է��ϼ��� : ");
        while (*searchGroup < 0) {
            printf("\t\t\t\t\t�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchGroup = getSearchInput("\t\t\t\t\t�˻��� �з� ��ȣ ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '3')) { // 3�� ������ ��� �˻��� �ش� �⵵ ���� �Է� ����
        *searchYear = getSearchInput("\t\t\t\t\t�˻��� �ش� �⵵ ���� �Է��ϼ��� : ");
        while (*searchYear < 0) {
            printf("\t\t\t\t\t�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchYear = getSearchInput("\t\t\t\t\t  �˻��� �ش� �⵵ ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '4')) { // 4�� ������ ��� �˻��� �ش� ���� ���� �Է� ����
        *searchMonth = getSearchInput("\t\t\t\t\t�˻��� �ش� ���� ���� �Է��ϼ��� : ");
        while (*searchMonth < 0) {
            printf("\t\t\t\t\t�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchMonth = getSearchInput("\t\t\t\t\t�˻��� �ش� ���� ���� �Է��ϼ��� : ");
        }
    }
    if (isDigitPresent(input, '5')) { // 5�� ������ ��� �˻��� �ش� ���� ���� �Է� ����
        *searchDay = getSearchInput("\t\t\t\t\t�˻��� �ش� ���� ���� �Է��ϼ��� : ");
        while (*searchDay < 0) {
            printf("\t\t\t\t\t�ùٸ��� ���� ���Դϴ�. �ٽ� �Է��ϼ���.\n");
            *searchDay = getSearchInput("\t\t\t\t\t�˻��� �ش� ���� ���� �Է��ϼ��� : ");
        }
    }
}


// ������ ��� �Լ�
void printPage(BookData arr[], int dataSize, int startIdx, int endIdx) {
    for (int i = startIdx; i <= endIdx; i++) {
        printf("\t\t\t\t\t\t%d. %14lld\n", i + 1, arr[i].code);
    }
}

// ��ü ���� �ڵ� ��� �Լ�
void printAllBooks(BookData arr[], int dataSize) {
    CLEAR_SCREEN();

    int totalPages = (dataSize + BOOKS_PER_PAGE - 1) / BOOKS_PER_PAGE; // �ø��� �̿��Ͽ� ������ �� ���
    int currentPage = 1;
    int startIdx = 0;
    int endIdx = (dataSize < BOOKS_PER_PAGE) ? dataSize - 1 : BOOKS_PER_PAGE - 1; // ������ �������� ���

    while (true) {
        printf("\t\t\t\t\t\t==== ���� �ڵ� ��� ====\n\n");
        printPage(arr, dataSize, startIdx, endIdx);
        printf("\n");

        // ���� ������ �Ǵ� ���� �������� �̵����� ���� ����
        char moveKey;

        if (currentPage == 1) { // ù �������� ��� ���� �������� �̵��ϴ� �ɼ� ��Ȱ��ȭ
            printf("\t\t\t\t\t\t  [ ������ %d / %d ]\n\n", currentPage, totalPages);
            printf("\t\t=======================================================================================\n");
            printf("\t\t||   ���� �������� �̵� �Ұ�   ||  ���� �������� �̵�[n]  ||         ����[q]         ||\n");
            printf("\t\t=======================================================================================\n");
            centerText("���� : ");
            scanf(" %c", &moveKey);
            if (moveKey == 'n' && currentPage < totalPages) {
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
        else if (currentPage == totalPages) { // ������ �������� ��� ���� �������� �̵��ϴ� �ɼ� ��Ȱ��ȭ
            printf("\t\t\t\t\t\t  [ ������ %d / %d ]\n\n", currentPage, totalPages);
            printf("\t\t=======================================================================================\n");
            printf("\t\t||   ���� �������� �̵�[p]   ||  ���� �������� �̵� �Ұ�  ||         ����[q]         ||\n");
            printf("\t\t=======================================================================================\n");
            centerText("���� : ");
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
        else { // ù ������, ������ �������� �ƴ� ��� ������������ ���������� ���� ����
            printf("\t\t\t\t\t\t  [ ������ %d / %d ]\n\n", currentPage, totalPages);
            printf("\t\t=======================================================================================\n");
            printf("\t\t||   ���� �������� �̵�[p]   ||   ���� �������� �̵�[n]   ||         ����[q]         ||\n");
            printf("\t\t=======================================================================================\n");
            centerText("���� : ");
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

// �����ڵ带 �˻��ϴ� �Լ�
void searchBooks(BookData arr[], int dataSize) {

    CLEAR_SCREEN();

    long long int searchCategory;
    long long int searchGroup;
    long long int searchYear;
    long long int searchMonth;
    long long int searchDay;

    printf("\n");
    centerText("           [ ���� �ڵ� �˻� ]           \n");
    centerText("========================================\n");
    centerText("||      �˻��� �׸��� �����ϼ���      ||\n");
    centerText("========================================\n");
    centerText("||       [ �׸� ���ù�� ���� ]       ||\n");
    centerText("||                                    ||\n");
    centerText("||     111,314,412,123,12345,���     ||\n");
    centerText("||     1~5 ������ ���� �Է��Ѵٸ�     ||\n");
    centerText("||      ��� �Է��ص� �������      ||\n");
    centerText("========================================\n");
    centerText("||                                    ||\n");
    centerText("||             1.�����з�             ||\n");
    centerText("||                                    ||\n");
    centerText("||             2.�з���ȣ             ||\n");
    centerText("||                                    ||\n");
    centerText("||             3.�⵵                 ||\n");
    centerText("||                                    ||\n");
    centerText("||             4.��                   ||\n");
    centerText("||                                    ||\n");
    centerText("||             5.��                   ||\n");
    centerText("||                                    ||\n");
    centerText("========================================\n");

    char input[20];

    // �˻��� �׸��� 
    while (true) { // ���� �ùٸ��� �Էµ� �� ���� �ݺ�
        centerText("�˻��� �׸��� �����ϼ���: ");
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

        if (validInput) { // �ùٸ� ���� �ԷµǸ� �ݺ��� ����
            break;
        }
        else { 
            centerText("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
            while (getchar() != '\n');  // �ùٸ��� ���� ���� �Էµ� ��� �ٽ� �Է��� ����
        }
    }

    // �Էµ� �� ���, � �׸��� �����ߴ��� ���
    printf("\t\t\t\t\t\t  �Էµ� ��: %s\n", input);

    // �ߺ��� ���� ó��
    removeDuplicateDigits(input);

    // ������ �׸񺰷� �ٽ� �˻��ϰ��� �ϴ� ���� �Է��Ͽ� Ž��
    promptUserForSearchCriteria(input, &searchCategory, &searchGroup, &searchYear, &searchMonth, &searchDay);

    // ���� �˻� ����
    bool found = false;
    int resultIndex = 0;

    // �˻� ��� ���
    CLEAR_SCREEN();
    centerText("========================================\n");
    centerText("[ �˻� ��� ]\n");
    centerText("========================================\n\n");
    int n = 0;

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
            printf("\t\t\t\t\t\t%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, arr[i].category, arr[i].group, arr[i].year, arr[i].month, arr[i].day);
            n++;
            found = true;
        }
    }
    printf("\n");
    centerText("========================================\n");

    if (!found) { // �˻���� ��ġ�ϴ� �����ڵ尡 ���ٸ� ��ġ�ϴ� �ڵ尡 ������ ���
        centerText("��ġ�ϴ� �����ڵ尡 �����ϴ�.\n");
        centerText("========================================\n");
        printf("\n");
    }
    else { // �˻���� ��ġ�ϴ� �����ڵ尡 ������ n���� �����ڵ尡 �˻��Ǿ����� ���
        printf("\t\t\t\t\t�� %d ���� �����ڵ尡 �˻��Ǿ����ϴ�.\n",n);
        centerText("========================================\n");
        printf("\n");
    }

}

// ���Ͽ� ���� �ڵ带 �߰��ϴ� �Լ�
void writeBookToFile(FILE* file, BookData book) {
    fprintf(file, "%03lld%03lld%04lld%02lld%02lld\n", book.category, book.group, book.year, book.month, book.day);
}

// ���������� ���� �Է��� ��� ��ü�ڵ带 �����ϱ� ���� ��ü�ڵ带 ����
void generateReplacementCode(char* replacementCode) {
    // localtime �Լ��� ���� ��¥�� �ҷ��´�.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // 'NSC(���� ��¥)' ���·� ��ü �ڵ� ����
    sprintf(replacementCode, "NSC%d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// �Էµ� ���� �ڵ尡 ��ȿ�� ������ Ȯ���ϴ� �Լ�
bool isValidCode(const char* input) {
    // ���̰� 14�ڸ����� Ȯ��
    if (strlen(input) != 14) {
        return false; // 14�ڸ����� ũ�ų� ������ �������� ���� ���̶� �Ǵ�
    }

    // ��� ���ڰ� �������� Ȯ��
    for (int i = 0; i < 14; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return false; // �Էµ� ���� 0�̻��� �������� �ƴ� �ٸ� ���� �Էµ� �������� ���� ���̶� �Ǵ�
        }
    }

    return true; // ��� �˻縦 ����ߴٸ� �ùٸ� ���̶� �Ǵ�
}

// ��¥�� ��ȿ���� Ȯ���ϴ� �Լ�
bool isValidDate(long long int year, long long int month, long long int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    } // �⵵���� 0���� �۰ų�, �� ���� 1~12�� ������ ���� �ƴѰ��, �� ���� 1���� ���� �����ΰ�� False�� ��ȯ

    // �� ���� �ϼ��� Ȯ���Ͽ� ��ȿ�� �˻�
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        // ������ ��� 2���� 29�ϱ��� ��ȿ
        daysInMonth[2] = 29;
    }

    return day <= daysInMonth[month]; // ���� �ٽ� ��ȯ
}

// ���ο� �����ڵ带 library.csv ���Ͽ� �߰��ϴ� �Լ�
void addBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    char inputCode[15];  // �ڵ带 �Է¹��� ���ڿ�, �ڵ�� 14�ڸ��̸� �� ���� ���ڸ� ����� 15�� ����
    long long int code;

    // 1�ܰ�: ���ڿ��� �߰��� ���� �ڵ� �Է�
    while (true) {
        printf("\n");
        centerText("[ ���� �ڵ� �߰� ]\n");
        centerText("========================================\n");
        centerText("||   �߰��Ͻ� �����ڵ带 �Է��ϼ���   ||\n");
        centerText("========================================\n");
        printf("\n\t\t\t\t\t�Է� : ");

        scanf("%s", inputCode);

        // 2�ܰ�: �Է��� ���ڿ� �Ǵ� 0 �̻��� ���� �������� Ȯ��
        if (isValidCode(inputCode)) {
            // 3�ܰ�: �Է� ������ �ڵ�� �ǴܵǸ� ���ڿ��� ���԰����ϰ� long long int�� ��ȯ
            code = atoll(inputCode);

            // 4�ܰ�: ��ȿ�� �ڵ�� �Ǵܵ� �ڵ忡�� ��¥�� �����ϴ� ��¥���� �Ǵ�
            long long int category, group, year, month, day;
            parseCode(code, &category, &group, &year, &month, &day);

            // 5-1�ܰ�: ��¥�� ��ȿ���� ���� ��� �ٽ� �Է� ����
            if (!isValidDate(year, month, day)) {
                CLEAR_SCREEN();
                printf("��ȿ���� ���� ��¥�Դϴ�. �ٽ� �Է��ϼ���.\n");
                continue;
            }

            // 5�ܰ� �ڵ尡 ��ȿ�� ���, �����ڵ带 ���Ͽ� �߰�
            FILE* file = fopen("library.csv", "a");
            if (file == NULL) {
                CLEAR_SCREEN();
                perror("������ �� �� �����ϴ�.");
                return;
            }

            // 6�ܰ�: library.csv ���Ͽ� �����ڵ带 ����
            writeBookToFile(file, (BookData) { code, category, group, year, month, day });

            // ���� �ݱ�
            fclose(file);

            printf("\n");
            centerText("========================================\n");
            centerText("������ ���������� �߰��Ǿ����ϴ�.\n");
            centerText("========================================\n");
            break;
        }
        else {
            // 3-1�ܰ�: ���������� �ڵ��� ��� ��ü �ڵ带 �����Ͽ� ���Ͽ� �߰�
            printf("\n");
            centerText("�߸��� �����ڵ� �����Դϴ�. ��ü �ڵ带 �����Ͽ� �߰��մϴ�.\n\n");

            char replacementCode[15];
            generateReplacementCode(replacementCode); // 3-2�ܰ�: ���������� �ڵ� ��� ������ ��ü�ڵ� ����

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

            printf("\t\t\t\t\t�߰��� ��ü�ڵ� : %s\n\n", replacementCode);
            break;
        }
    }
}

// �����ڵ带 ������ �迭���� �����ϴ� �Լ�
void modifyBook(BookData arr[], int dataSize) {
    CLEAR_SCREEN();
    char searchCode[20];
    long long int code;
    char confirmModify;
    char newCode[20];
    long long int intNewCode;

    // 1�ܰ�: �����ϰ��� �ϴ� �ڵ尡 ���� ���� �ִ��� Ž��
    printf("\n");
    centerText("[ ���� �ڵ� ���� ]\n");
    centerText("========================================\n");
    centerText("||   �����Ͻ� �����ڵ带 �Է��ϼ���   ||\n");
    centerText("========================================\n");

    while (true) { // 2�ܰ�: �ùٸ� ���� �Էµ� ������ while�� ���� ������ �ݺ�
        printf("\n\t\t\t\t\t�Է� : ");

        scanf("%s", searchCode);

        // 2�ܰ�: �Էµ� ���� ��ȿ�� �ڵ����� �Ǵ�
        if (isValidCode(searchCode)) { // 3�ܰ�: ��ȿ�� �ڵ�� �ǴܵǴ� ��� ����
            // 3�ܰ�: �����ڵ� ���� �˻��ϰ� �ڵ带 ������ �� �ְ� ���ڿ��� long long int�� ��ȯ
            code = atoll(searchCode);

            // 4�ܰ�: ��ȿ�� �ڵ�� �Ǵܵ� �ڵ忡�� ��¥�� �����ϴ� ��¥���� �Ǵ�
            long long int category, group, year, month, day;
            parseCode(code, &category, &group, &year, &month, &day);

            // 4-1�ܰ�: ��¥�� ��ȿ���� ���� ��� �ٽ� �Է� ����
            if (!isValidDate(year, month, day)) {
                CLEAR_SCREEN();
                printf("��ȿ���� ���� ��¥�Դϴ�. �ٽ� �Է��ϼ���.\n");
                continue;  // ��¥�� ��ȿ���� ������ �ݺ��� ó������ ���ư�
            }
            printf("\n");
            centerText("������ �����ڵ尡 �߰ߵǾ����ϴ�.\n");
            break;
        } 
        else { // 2-1�ܰ�: �ùٸ��� ���� ���̶� �ǴܵǴ°�� �ٽ� �Է��� ����
            printf("\n");
            centerText("�߸��� �����ڵ� �����Դϴ�. �ٽ� �Է��ϼ���.\n\n");
            continue;  // �߸��� �����ڵ� �����̸� �ݺ��� ó������ ���ư�
        }
    }

    // 5�ܰ�: ��� �˻翡�� ������ �ڵ�� �ǴܵǸ� ���� ���� ��ġ�ϴ� ���� �˻�, ã�´ٸ� �� ���� �ε��� ��ġ ���� ����
    int foundIndex = -1;
    for (int i = 0; i < dataSize; i++) {
        if (arr[i].code == code) {
            foundIndex = i;
            break;
        }
    }


    // 6-1�ܰ�: ���� ������ ã�����ϴ� �ڵ尡 ���� ���
    if (foundIndex == -1) {
        printf("\n");
        centerText("�ش� �����ڵ尡 �������� �ʽ��ϴ�.\n");

        char retry;
        printf("\n");
        centerText("�ٽ� �Է��Ͻðڽ��ϱ�? [y/n]: ");
        scanf(" %c", &retry);

        // 6-2�ܰ�: �ٽ� �Է��� ������ Ȯ��
        if (retry == 'y') {            // �ٽ� �Է��� �����ϴ� ���
            modifyBook(arr, dataSize); // ��� ȣ���� ���� �Լ� �����
        }
        else {
            printf("���α׷��� �����մϴ�.\n");
            return; // ����
        }
    }
    else { 
        do { // 6�ܰ�: ���� ������ ��ġ�ϴ� ���� ã�� ��� �ùٸ� �����ڵ带 �Է��� ������ �ݺ�
            printf("\n");
            printf("\n\t\t\t\t    ���ο� �����ڵ带 �Է��ϼ���: ");
            scanf("%s", newCode); // 7�ܰ�: ���� ���� ������ �����ڵ带 ��� �ٲ��� �Է�

            // 8�ܰ�: ������ �����ڵ� ���� ��� �������� Ȯ��
            if (!isValidCode(newCode)) { // 9-1�ܰ�: ���Ұ����� �����ڵ�� �ǴܵǴ°�� �ٽ� �Է��� ����
                printf("\n");
                centerText("��ȿ���� ���� �����ڵ��Դϴ�. �ٽ� �Է��ϼ���.\n");
            }
            else {
                // 9�ܰ�: ��밡���� �ڵ�� �ǴܵǸ�, �Էµ� ��¥�� ������ �����ϴ��� Ȯ��
                intNewCode = atoll(newCode);
                long long int category, group, year, month, day;
                parseCode(intNewCode, &category, &group, &year, &month, &day);

                if (!isValidDate(year, month, day)) { // 10-1�ܰ�: ��¥�� �������� �ʴ´ٸ� �ٽ� �Է��� ����
                    printf("\n");
                    centerText("��ȿ���� ���� ��¥�Դϴ�. �ٽ� �Է��ϼ���.\n");
                }
                else { // 10�ܰ�: ��¥�� �����Ѵٸ� �����ڵ带 ����
                    printf("\n\t\t\t    �����ڵ� %lld�� %lld�� �����Ͻðڽ��ϱ�? [y/n]: ", code, intNewCode);
                    scanf(" %c", &confirmModify);

                    if (confirmModify == 'y') {
                        arr[foundIndex].code = intNewCode;
                        printf("\n");
                        centerText("========================================\n");
                        centerText("||�����ڵ尡 ���������� �����Ǿ����ϴ�||\n");
                        centerText("========================================\n");

                        // 11�ܰ�: ���� ��� ������ �ڵ�� ������
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
                            if (currentCode == code) {
                                // ������ �ڵ�� ��ü
                                fprintf(tempFile, "%lld\n", intNewCode);
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
                    else {
                        printf("\n");
                        centerText("========================================\n");
                        centerText("||  �����ڵ� ������ ��� �Ǿ����ϴ�.  ||\n");
                        centerText("========================================\n");
                        break;
                    }
                }
            }
        } while (true);
    }
}

// �����ڵ带 library.csv���� �����ϴ� �Լ�
void deleteBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    long long int searchCode;

    // 1�ܰ�: ������ �����ڵ带 �Է�
    printf("\n");
    centerText("[ ���� �ڵ� ���� ]\n");
    centerText("========================================\n");
    centerText("||   �����Ͻ� �����ڵ带 �Է��ϼ���   ||\n");
    centerText("========================================\n");
    printf("\n\t\t\t\t\t�Է� : ");
    scanf("%lld", &searchCode);

    // 2�ܰ�: ���� ���� �����ڵ尡 �ִ��� �˻�
    int foundIndex = -1;
    for (int i = 0; i < *dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    // 3-1�ܰ�: �����ڵ尡 ���� ��� ����
    if (foundIndex == -1) {
        printf("\n");
        centerText("�ش� �����ڵ尡 �������� �ʽ��ϴ�.\n");

        char retry;
        printf("\n");
        centerText("�ٽ� �Է��Ͻðڽ��ϱ�? [y/n]: ");
        scanf(" %c", &retry);

        // 3-2�ܰ�: �ٽ� �Է��� ������ Ȯ��
        if (retry == 'y') {
            deleteBook(arr, dataSize); // ��� ȣ���� ���� �Լ� �����
        }
        else {
            printf("\n");
            centerText("========================================\n");
            centerText("||       ���α׷��� �����մϴ�.       ||\n");
            centerText("========================================\n");
            return; // ����
        }
    }
    else { // 3�ܰ�: ���� ���� ������ �����ڵ尡 �ִ°�� ����
        char confirmDelete;
        // 4�ܰ�: ���� ���� �ٽ� Ȯ��
        printf("\n");
        printf("\n\t\t\t\t�����ڵ� %lld�� �����Ͻðڽ��ϱ�? [y/n]: ", searchCode);
        scanf(" %c", &confirmDelete);

        if (confirmDelete == 'y') {
            // 5�ܰ�: ���� ����
            for (int i = foundIndex; i < *dataSize - 1; i++) {
                arr[i] = arr[i + 1];
            }

            (*dataSize)--;
            printf("\n");
            centerText("========================================\n");
            centerText("||�����ڵ尡 ���������� �����Ǿ����ϴ�||\n");
            centerText("========================================\n");

            // 6�ܰ�: ���� ��� ������ �ڵ带 'library.csv' ���Ͽ��� ����
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

            // ���� ������ �о ������ �ڵ带 'library.csv' ���Ͽ��� ����
            long long int currentCode;
            while (fscanf(file, "%lld", &currentCode) == 1) {
                if (currentCode != searchCode) {
                    // �������� ���� �ڵ常 �ӽ� ���Ͽ� ����
                    fprintf(tempFile, "%lld\n", currentCode);
                }
            }

            // ���� �ݱ�
            fclose(file);
            fclose(tempFile);

            // ������ �Ϸ�� �ӽ� ������ ���� ���Ϸ� ����
            remove("library.csv"); // ���� ���� ����
            rename("temp.csv", "library.csv"); // �ӽ� ������ ���� ���Ϸ� �̸� ����
        }
        else {
            printf("\n");
            centerText("========================================\n");
            centerText("||   �����ڵ� ������ ��ҵǾ����ϴ�   ||\n");
            centerText("========================================\n");
        }
    }
}

// ���� �޴� UI ���� �Լ�
void showMainMenu() {
    printf("\n\n");
    centerText("========== �˰��� ���� C�� ��������Ʈ 3�� ==========\n");
    centerText("||   [ �赿�� ]       [ ������ ]       [ ��âȣ ]   ||\n");
    centerText("======================================================\n");
    centerText("||                                                  ||\n");
    centerText("||              [ ���� ���� ���α׷� ]              ||\n");
    centerText("||                                                  ||\n");
    centerText("======================================================\n");
    centerText("||                                                  ||\n");
    centerText("||                 1. �����ڵ� ���                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 2. �����ڵ� �˻�                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 3. �����ڵ� �߰�                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 4. �����ڵ� ����                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 5. �����ڵ� ����                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 0. ���α׷� ����                 ||\n");
    centerText("||                                                  ||\n");
    centerText("======================================================\n");
    centerText("======================================================\n");
    printf("\n");
}

// ���θ޴� ���� �Լ�
int runMainMenu(BookData arr[], int dataSize) {
    int choice = -1;

    do {
        showMainMenu();
        centerText("���ϴ� �۾��� �����ϼ���: ");

        if (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            // ������ �ƴϰų� 0���� 5 ������ ���� �ƴ� ���
            CLEAR_SCREEN();
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            choice = -1; // �ٽ� �Է� �ޱ� ���� choice�� -1�� ����
            continue; // �ٽ� �ݺ����� ó������ �̵�
        }

        // ����� ����� ����
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
            addBook(arr, &dataSize);
            break;
        case 4:
            // �����ڵ� ����
            modifyBook(arr, dataSize);
            break;
        case 5:
            // �����ڵ� ����
            deleteBook(arr, &dataSize);
            break;
        case 0:
            // ���α׷� ����
            printf("���α׷��� �����մϴ�.\n");
            break;
        default:
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
        }

        if (choice != 0) {
            char returnToMain;
            printf("\n");
            printf("���� ȭ������ ���ư��ðڽ��ϱ�? [y/n] : ");
            scanf(" %c", &returnToMain);

            if (returnToMain != 'y') {
                choice = 0;
            }
            else {
                CLEAR_SCREEN();
            }
        }

    } while (choice != 0);

    return choice;
}

int main() { // ���� �Լ�
    // library.csv ���� ����
    FILE* file = fopen("library.csv", "r");
    if (file == NULL) {
        perror("������ �� �� �����ϴ�.");
        return 1;
    }

    // ������ �迭 �ʱ� ũ�� ����
    int initialSize = 30000;

    // ����ü ������ �迭 ���� �Ҵ�
    BookData* data = malloc(initialSize * sizeof(BookData));
    if (data == NULL) {
        perror("�޸𸮸� �Ҵ��� �� �����ϴ�.");
        fclose(file);
        return 1;
    }

    // ������ ������ ���� �迭 ũ�� �ʱ�ȭ
    int dataSize = 0;
    int currentSize = initialSize;

    // ���Ͽ��� ������ �о�� ����ü�� �� �׸� �� ����
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

    // ������ �� ����ߴٸ� �ݱ�
    fclose(file);

    // �о�� �����͸� �� ������ ���� ������� �ٽ� ����
    quickSort(data, 0, dataSize - 1);

    int continueProgram;

    do { // ���� �޴��� ��� �����ְ� ����
        continueProgram = runMainMenu(data, dataSize);
    } while (continueProgram != 0);

    // �Ҵ�� �޸� ����
    free(data);
    CLEAR_SCREEN();
    printf("���α׷��� ����Ǿ����ϴ�.");
    return 0;
}