#define _CRT_SECURE_NO_WARNINGS
// 페이지에 표시할 도서의 수
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
    long long int category;   // 도서분류
    long long int code;       // 분류기호
    long long int year;       // 연도
    long long int month;      // 월
    long long int day;        // 일
} BookData;

// 퀵 정렬 함수
void swap(BookData* a, BookData* b) {
    BookData temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(BookData arr[], int left, int right) {
    int i = left, j = right;
    BookData pivot = arr[(left + right) / 2];

    // 분할
    while (i <= j) {
        while (arr[i].category < pivot.category || (arr[i].category == pivot.category && arr[i].code < pivot.code) || (arr[i].category == pivot.category && arr[i].code == pivot.code && (arr[i].year < pivot.year || (arr[i].year == pivot.year && (arr[i].month < pivot.month || (arr[i].month == pivot.month && arr[i].day < pivot.day))))))
            i++;
        while (arr[j].category > pivot.category || (arr[j].category == pivot.category && arr[j].code > pivot.code) || (arr[j].category == pivot.category && arr[j].code == pivot.code && (arr[j].year > pivot.year || (arr[j].year == pivot.year && (arr[j].month > pivot.month || (arr[j].month == pivot.month && arr[j].day > pivot.day))))))
            j--;
        if (i <= j) {
            // Swap 호출
            swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

// 각 자릿수를 분리하여 구조체 멤버에 저장
void parseCode(long long int code, long long int* category, long long int* codeNum, long long int* year, long long int* month, long long int* day) {
    *category = code / 100000000000;          // 10억으로 나누면 천의 자리가 남음
    *codeNum = (code / 100000000) % 100000;   // 백만으로 나눈 나머지를 천으로 나누면 백의 자리가 남음
    *year = (code % 100000000) / 10000;       // 10000으로 나누면 연도가 나옴
    *month = (code % 10000) / 100;            // 100으로 나누면 월이 나옴
    *day = code % 100;                        // 100으로 나눈 나머지가 일
}


// 검색 결과를 형식에 맞게 출력
void printSearchResult(int resultIndex, BookData book) {
    printf("%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, book.category, book.code, book.year, book.month, book.day);
}

// 중복된 숫자를 처리하고 고유한 숫자로 만드는 함수
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

    input[currentIndex] = '\0'; // 문자열 끝에 널 문자 추가
}

// 검색 입력을 받는 함수
long long int getSearchInput(const char* prompt) {
    long long int input;
    while (true) {
        printf("%s", prompt);
        if (scanf("%lld", &input) == 1) {
            break;
        }
        else {
            printf("잘못된 입력입니다. 다시 입력하세요.\n");
            while (getchar() != '\n');  // 버퍼 비우기
        }
    }
    return input;
}

// 각 숫자가 입력에 존재하는지 확인하는 함수
int isDigitPresent(char* input, char digit) {
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == digit) {
            return 1; // 숫자가 존재함
        }
    }
    return 0; // 숫자가 존재하지 않음
}

// 각 숫자에 따라 사용자에게 검색 기준을 입력 받는 함수
void promptUserForSearchCriteria(char* input, long long int* searchCategory, long long int* searchCode, long long int* searchYear, long long int* searchMonth, long long int* searchDay) {
    if (isDigitPresent(input, '1')) {
        *searchCategory = getSearchInput("검색하려는 도서분류 값을 입력하세요 : ");
        while (*searchCategory < 0) {
            printf("올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchCategory = getSearchInput("검색하려는 도서분류 값을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '2')) {
        *searchCode = getSearchInput("검색하려는 분류기호 값을 입력하세요 : ");
        while (*searchCode < 0) {
            printf("올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchCode = getSearchInput("검색하려는 분류기호 값을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '3')) {
        *searchYear = getSearchInput("검색하려는 해당 년도를 입력하세요 : ");
        while (*searchYear < 0) {
            printf("올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchYear = getSearchInput("검색하려는 해당 년도를 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '4')) {
        *searchMonth = getSearchInput("검색하려는 월을 입력하세요 : ");
        while (*searchMonth < 0) {
            printf("올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchMonth = getSearchInput("검색하려는 월을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '5')) {
        *searchDay = getSearchInput("검색하려는 해당일을 입력하세요 : ");
        while (*searchDay < 0) {
            printf("올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchDay = getSearchInput("검색하려는 해당일을 입력하세요 : ");
        }
    }
}

// 페이지 출력 함수
void printPage(BookData arr[], int dataSize, int startIdx, int endIdx) {
    for (int i = startIdx; i <= endIdx; i++) {
        printf("%d. %03lld%03lld%04lld%02lld%02lld\n", i + 1, arr[i].category, arr[i].code, arr[i].year, arr[i].month, arr[i].day);
    }
}

// 전체 도서 코드 출력 함수
void printAllBooks(BookData arr[], int dataSize) {
    CLEAR_SCREEN();

    int totalPages = (dataSize + BOOKS_PER_PAGE - 1) / BOOKS_PER_PAGE; // 올림을 이용하여 페이지 수 계산

    int currentPage = 1;
    int startIdx = 0;
    int endIdx = BOOKS_PER_PAGE - 1;

    while (true) {
        printPage(arr, dataSize, startIdx, endIdx);

        // 다음 페이지 또는 이전 페이지로 이동할지 여부 묻기
        char moveKey;
        if (currentPage == 1 && totalPages > 1) {
            // 첫 페이지인 경우 다음 페이지로만 이동 가능
            printf("[ 도서코드 전체 리스트 (페이지 %d / %d) ]\n", currentPage, totalPages);
            printf("\n다음 페이지로 이동[n] 종료[q]\n");
            printf("선택: ");
            scanf(" %c", &moveKey);
            if (moveKey == 'n') {
                startIdx += BOOKS_PER_PAGE;
                endIdx = (endIdx + BOOKS_PER_PAGE < dataSize - 1) ? endIdx + BOOKS_PER_PAGE : dataSize - 1;
                currentPage++;
                CLEAR_SCREEN();
            }
            else if (moveKey == 'q') {
                break;
            }
        }
        else if (currentPage == totalPages && totalPages > 1) {
            // 마지막 페이지인 경우 이전 페이지로만 이동 가능
            printf("[ 도서코드 전체 리스트 (페이지 %d / %d) ]\n", currentPage, totalPages);
            printf("\n이전 페이지로 이동[p] 종료[q]\n");
            printf("선택: ");
            scanf(" %c", &moveKey);
            if (moveKey == 'p') {
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
            // 그 외의 페이지에서는 이전페이지와 다음페이지 선택 가능
            printf("[ 도서코드 전체 리스트 (페이지 %d / %d) ]\n", currentPage, totalPages);
            printf("\n다음 페이지로 이동[n] 이전 페이지로 이동[p] 종료[q]\n");
            printf("선택: ");
            char moveKey;
            scanf(" %c", &moveKey);
            if (moveKey == 'p' && currentPage > 1) {
                // 이전 페이지로 이동
                startIdx -= BOOKS_PER_PAGE;
                endIdx = startIdx + BOOKS_PER_PAGE - 1;
                currentPage--;
                CLEAR_SCREEN();
            }
            else if (moveKey == 'n' && currentPage < totalPages) {
                // 다음 페이지로 이동
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

// 검색 함수
void searchBooks(BookData arr[], int dataSize) {

    CLEAR_SCREEN();

    long long int searchCategory;
    long long int searchCode;
    long long int searchYear;
    long long int searchMonth;
    long long int searchDay;

    printf("검색하려는 항목을 선택하세요:\n");
    printf("1. 도서분류\n");
    printf("2. 분류기호\n");
    printf("3. 년도\n");
    printf("4. 월\n");
    printf("5. 일\n");

    char input[20];

    // 사용자 입력 받기
    while (true) {
        printf("숫자를 입력하세요: ");
        scanf("%s", input);

        // 입력된 각 숫자를 확인하여 유효성 검사
        bool validInput = true;
        for (int i = 0; i < strlen(input); i++) {
            int digit = input[i] - '0';
            if (digit < 1 || digit > 5) {
                validInput = false;
                break;
            }
        }

        if (validInput) {
            // 올바른 값이 입력되면 반복문 종료
            break;
        }
        else {
            printf("잘못된 입력입니다. 다시 입력하세요.\n");
            while (getchar() != '\n');  // 버퍼 비우기
        }
    }

    // 입력된 값 출력 또는 저장할 수 있도록 원하는 동작 수행
    printf("입력된 값: %s\n", input);

    // 중복된 숫자 처리
    removeDuplicateDigits(input);

    // 입력에 따라 사용자에게 검색 기준 입력 받음
    promptUserForSearchCriteria(input, &searchCategory, &searchCode, &searchYear, &searchMonth, &searchDay);

    // 실제 검색 수행
    bool found = false;
    int resultIndex = 0;

    printf("[ 검색 결과 ]\n");

    // 검색된 도서코드를 출력
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
        printf("일치하는 도서코드가 없습니다.\n");
    }

}
// 파일에 도서 코드를 추가하는 함수
void writeBookToFile(FILE* file, BookData book) {
    fprintf(file, "%03lld%03lld%04lld%02lld%02lld\n", book.category, book.code, book.year, book.month, book.day);
}

// 비정상적인 값을 뜻하는 대체 코드 생성
void generateReplacementCode(char* replacementCode) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // 'NSC(현재 날짜)' 형태로 대체 코드 생성
    sprintf(replacementCode, "NSC%d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// 도서 코드의 유효성을 확인하는 함수
bool isValidCode(const char* input) {
    // 길이가 14자리인지 확인
    if (strlen(input) != 14) {
        return false;
    }

    // 모든 문자가 숫자인지 확인
    for (int i = 0; i < 14; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return false;
        }
    }

    return true;
}

// 날짜가 유효한지 확인하는 함수
bool isValidDate(long long int year, long long int month, long long int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    // 각 월의 일수를 확인하여 유효성 검사
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        // 윤년인 경우 2월은 29일까지 유효
        daysInMonth[2] = 29;
    }

    return day <= daysInMonth[month];
}

// 새로운 도서를 데이터 배열에 추가하는 함수
void addBook(BookData arr[], int* dataSize, int* currentSize) {
    CLEAR_SCREEN();

    // 새로운 도서 정보 입력
    char input[20];
    printf("추가할 새로운 도서의 도서코드를 입력하세요: ");
    scanf("%s", input);

    // 입력 값이 정상적인지 확인
    if (!isValidCode(input)) {
        printf("입력된 값이 비정상적입니다. 대체 코드를 생성하여 추가합니다.\n");
        generateReplacementCode(input);

        FILE* file = fopen("library.csv", "a");
        if (file == NULL) {
            perror("파일을 열 수 없습니다.");
            return;
        }

        // 대체 코드를 파일에 삽입
        fprintf(file, "%s\n", input);

        fclose(file);

        // 삽입된 도서 코드 대신 삽입된 비정상적인 코드 출력
        printf("도서가 추가되었습니다. 삽입된 대체 코드: %s\n", input);

        (*dataSize)++;
        return;
    }

    // 배열 크기가 부족하면 두 배로 확장
    if (*dataSize == *currentSize) {
        *currentSize *= 2;
        arr = realloc(arr, *currentSize * sizeof(BookData));
        if (arr == NULL) {
            perror("메모리를 재할당할 수 없습니다.");
            return;
        }
    }

    // 새로운 도서 정보를 배열에 추가
    arr[*dataSize].code = atoll(input);
    parseCode(arr[*dataSize].code, &arr[*dataSize].category, &arr[*dataSize].code, &arr[*dataSize].year, &arr[*dataSize].month, &arr[*dataSize].day);

    // 날짜 유효성 검사
    while (!isValidDate(arr[*dataSize].year, arr[*dataSize].month, arr[*dataSize].day)) {
        printf("유효하지 않은 날짜입니다. 다시 입력하세요.\n");

        // 날짜를 다시 입력
        printf("년도를 입력하세요: ");
        scanf("%lld", &arr[*dataSize].year);
        printf("월을 입력하세요: ");
        scanf("%lld", &arr[*dataSize].month);
        printf("일을 입력하세요: ");
        scanf("%lld", &arr[*dataSize].day);
    }

    // 파일에 도서 코드 추가
    FILE* file = fopen("library.csv", "a");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return;
    }

    // 도서 코드를 파일에 삽입하는 부분 추가
    writeBookToFile(file, arr[*dataSize]);

    fclose(file);

    // 삽입된 도서 코드 출력
    printf("도서가 추가되었습니다. 삽입된 도서 코드: %03lld%03lld%04lld%02lld%02lld\n",
        arr[*dataSize].category, arr[*dataSize].code, arr[*dataSize].year, arr[*dataSize].month, arr[*dataSize].day);

    (*dataSize)++;
}

// 도서를 데이터 배열에서 수정하는 함수
void modifyBook(BookData arr[], int dataSize) {

    CLEAR_SCREEN();

    char input[20];

    while (true) {
        printf("수정하려는 도서의 도서코드를 입력하세요: ");
        scanf("%s", input);

        // 입력된 코드가 유효한지 확인
        if (!isValidCode(input)) {
            CLEAR_SCREEN();
            printf("입력된 값이 비정상적입니다. 올바른 도서코드를 입력하세요.\n");
            continue; // 사용자에게 코드를 다시 입력하도록 요청
        }

        long long int searchCode = atoll(input); // 유효한 코드를 long long int로 변환

        int foundIndex = -1;
        for (int i = 0; i < dataSize; i++) {
            if (arr[i].code == searchCode) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            // 수정 사항을 입력받습니다.
            printf("도서분류 수정: ");
            scanf("%lld", &arr[foundIndex].category);
            printf("분류기호 수정: ");
            scanf("%lld", &arr[foundIndex].code);
            printf("년도 수정: ");
            scanf("%lld", &arr[foundIndex].year);
            printf("월 수정: ");
            scanf("%lld", &arr[foundIndex].month);
            printf("일 수정: ");
            scanf("%lld", &arr[foundIndex].day);

            printf("도서 정보가 수정되었습니다.\n");

            // 사용자에게 다시 입력 여부를 물어봅니다.
            char retryInput;
            printf("다시 입력하시겠습니까? [y/n]: ");
            scanf(" %c", &retryInput);

            if (retryInput == 'y') {
                // 다시 입력하려면 처음으로 돌아갑니다.
                CLEAR_SCREEN();
                continue;
            }
            else {
                // 종료하려면 루프를 종료합니다.
                break;
            }
        }
        else {
            CLEAR_SCREEN();
            printf("일치하는 도서코드가 없습니다.\n");

            // 사용자에게 다시 입력 여부를 물어봅니다.
            char retryInput;
            printf("다시 입력하시겠습니까? [y/n]: ");
            scanf(" %c", &retryInput);

            if (retryInput == 'y') {
                // 다시 입력하려면 처음으로 돌아갑니다.
                continue;
            }
            else {
                // 종료하려면 루프를 종료합니다.
                break;
            }
        }
    }
}

// 도서를 데이터 배열에서 삭제하는 함수
void deleteBook(BookData arr[], int* dataSize) {

    CLEAR_SCREEN();

    long long int searchCode;
    printf("삭제하려는 도서의 도서코드를 입력하세요: ");
    scanf("%lld", &searchCode);

    int foundIndex = -1;
    for (int i = 0; i < *dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        // 마지막 도서를 삭제된 위치로 이동하고 dataSize를 감소시킵니다.
        arr[foundIndex] = arr[*dataSize - 1];
        (*dataSize)--;
        printf("도서가 삭제되었습니다.\n");
    }
    else {
        printf("일치하는 도서코드가 없습니다.\n");
    }
}

// 메인 메뉴 함수
void showMainMenu() {
    printf("도서 관리 프로그램\n");
    printf("1. 도서코드 전체 리스트 출력\n");
    printf("2. 도서코드 검색\n");
    printf("3. 도서코드 추가\n");
    printf("4. 도서코드 수정\n");
    printf("5. 도서코드 삭제\n");
    printf("0. 프로그램 종료\n");
}

int runMainMenu(BookData arr[], int* dataSize, int* currentSize) {
    int choice = -1;

    do {
        showMainMenu();
        printf("원하는 작업을 선택하세요: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // 도서코드 전체 리스트 확인
            printAllBooks(arr, *dataSize);
            break;
        case 2:
            // 도서코드 검색
            searchBooks(arr, *dataSize);
            break;
        case 3:
            // 도서코드 추가
            addBook(arr, dataSize, currentSize);
            break;
        case 4:
            // 도서코드 수정
            modifyBook(arr, *dataSize);
            break;
        case 5:
            // 도서코드 삭제
            deleteBook(arr, dataSize);
            break;
        case 0:
            // 프로그램 종료
            printf("프로그램을 종료합니다.\n");
            break;
        default:
            printf("잘못된 입력입니다. 다시 입력하세요.\n");
            break;
        }

        if (choice != 0) {
            // 사용자에게 메인 메뉴로 돌아가겠냐는 여부를 물어봅니다.
            char returnToMain;
            printf("메인 화면으로 돌아가시겠습니까? [y/n] : ");
            scanf(" %c", &returnToMain);

            if (returnToMain != 'y') {
                choice = 0; // 사용자가 돌아가기를 원하지 않으면 루프를 종료하여 프로그램 종료
            }
            else {
                // 선택이 'y'인 경우, 메인 화면으로 돌아가기 전에 화면을 지우도록 설정
                CLEAR_SCREEN();
            }
        }

    } while (choice != 0); // 0은 프로그램을 종료합니다.

    // 계속할지 종료할지를 나타내는 값을 반환
    return choice;
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
    BookData* data = malloc(initialSize * sizeof(BookData));
    if (data == NULL) {
        perror("메모리를 할당할 수 없습니다.");
        fclose(file);
        return 1;
    }

    // 데이터 개수와 현재 배열 크기 초기화
    int dataSize = 0;
    int currentSize = initialSize;

    // 파일에서 데이터 읽기
    while (fscanf(file, "%lld", &data[dataSize].code) == 1) {
        parseCode(data[dataSize].code, &data[dataSize].category, &data[dataSize].code, &data[dataSize].year, &data[dataSize].month, &data[dataSize].day);
        dataSize++;

        // 배열 크기가 부족하면 두 배로 확장
        if (dataSize == currentSize) {
            currentSize *= 2;
            data = realloc(data, currentSize * sizeof(BookData));
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

    int continueProgram;

    do {
        continueProgram = runMainMenu(data, &dataSize, &currentSize);
    } while (continueProgram != 0);
    
    // 할당된 메모리 해제
    free(data);

    return 0;
}