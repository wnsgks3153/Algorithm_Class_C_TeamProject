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
    long long int code;       // 도서코드전체(14자리)
    long long int category;   // 도서분류
    long long int group;      // 분류기호
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
        while (arr[i].category < pivot.category || (arr[i].category == pivot.category && arr[i].group < pivot.group) || (arr[i].category == pivot.category && arr[i].group == pivot.group && (arr[i].year < pivot.year || (arr[i].year == pivot.year && (arr[i].month < pivot.month || (arr[i].month == pivot.month && arr[i].day < pivot.day))))))
            i++;
        while (arr[j].category > pivot.category || (arr[j].category == pivot.category && arr[j].group > pivot.group) || (arr[j].category == pivot.category && arr[j].group == pivot.group && (arr[j].year > pivot.year || (arr[j].year == pivot.year && (arr[j].month > pivot.month || (arr[j].month == pivot.month && arr[j].day > pivot.day))))))
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
void parseCode(long long int code, long long int* category, long long int* group, long long int* year, long long int* month, long long int* day) {
    *category = code / 100000000000;          // 10억으로 나누면 천의 자리가 남음
    *group = (code / 100000000) % 100000;   // 백만으로 나눈 나머지를 천으로 나누면 백의 자리가 남음
    *year = (code % 100000000) / 10000;       // 10000으로 나누면 연도가 나옴
    *month = (code % 10000) / 100;            // 100으로 나누면 월이 나옴
    *day = code % 100;                        // 100으로 나눈 나머지가 일
}


// 검색 결과를 형식에 맞게 출력
void printSearchResult(int resultIndex, BookData book) {
    printf("%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, book.category, book.group, book.year, book.month, book.day);
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
void promptUserForSearchCriteria(char* input, long long int* searchCategory, long long int* searchGroup, long long int* searchYear, long long int* searchMonth, long long int* searchDay) {
    if (isDigitPresent(input, '1')) {
        *searchCategory = getSearchInput("검색하려는 도서분류 값을 입력하세요 : ");
        while (*searchCategory < 0) {
            printf("올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchCategory = getSearchInput("검색하려는 도서분류 값을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '2')) {
        *searchGroup = getSearchInput("검색하려는 분류기호 값을 입력하세요 : ");
        while (*searchGroup < 0) {
            printf("올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchGroup = getSearchInput("검색하려는 분류기호 값을 입력하세요 : ");
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
        printf("%d. %03lld%03lld%04lld%02lld%02lld\n", i + 1, arr[i].category, arr[i].group, arr[i].year, arr[i].month, arr[i].day);
    }
}

// 전체 도서 코드 출력 함수
void printAllBooks(BookData arr[], int dataSize) {
    CLEAR_SCREEN();

    int totalPages = (dataSize + BOOKS_PER_PAGE - 1) / BOOKS_PER_PAGE; // 올림을 이용하여 페이지 수 계산

    int currentPage = 1;
    int startIdx = 0;
    int endIdx = (dataSize < BOOKS_PER_PAGE) ? dataSize - 1 : BOOKS_PER_PAGE - 1;

    while (true) {
        printPage(arr, dataSize, startIdx, endIdx);

        // 다음 페이지 또는 이전 페이지로 이동할지 여부 묻기
        char moveKey;
        if (currentPage == totalPages) {
            // 마지막 페이지인 경우 이전 페이지로만 이동 가능
            printf("[ 도서코드 전체 리스트 (페이지 %d / %d) ]\n", currentPage, totalPages);
            printf("\n이전 페이지로 이동[p] 종료[q]\n");
            printf("선택: ");
            scanf(" %c", &moveKey);
            if (moveKey == 'p' && currentPage > 1) {
                // 이전 페이지로 이동
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
    long long int searchGroup;
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
    promptUserForSearchCriteria(input, &searchCategory, &searchGroup, &searchYear, &searchMonth, &searchDay);

    // 실제 검색 수행
    bool found = false;
    int resultIndex = 0;

    CLEAR_SCREEN();
    printf("[ 검색 결과 ]\n");

    // 검색된 도서코드를 출력
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
        printf("일치하는 도서코드가 없습니다.\n");
    }

}

// 파일에 도서 코드를 추가하는 함수
void writeBookToFile(FILE* file, BookData book) {
    fprintf(file, "%03lld%03lld%04lld%02lld%02lld\n", book.category, book.group, book.year, book.month, book.day);
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
void addBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    char inputCode[15];  // 코드를 입력받을 문자열, 코드는 14자리이며 널 종료 문자를 고려해 15로 설정
    long long int code;

    // 단계 1: 도서 코드 입력 (문자열로)
    while (true) {
        printf("도서코드를 입력하세요: ");
        scanf("%s", inputCode);

        // 단계 2: 입력이 문자열 또는 양의 정수인지 확인
        if (isValidCode(inputCode)) {
            // 문자열을 long long int로 변환
            code = atoll(inputCode);

            // 단계 3: 코드에 유효한 날짜가 있는지 확인
            long long int category, group, year, month, day;
            parseCode(code, &category, &group, &year, &month, &day);

            // 날짜가 유효하지 않은 경우 다시 입력 받음
            if (!isValidDate(year, month, day)) {
                CLEAR_SCREEN();
                printf("유효하지 않은 날짜입니다. 다시 입력하세요.\n");
                continue;
            }

            // 코드가 유효한 경우, 책을 파일에 추가
            FILE* file = fopen("library.csv", "a");
            if (file == NULL) {
                CLEAR_SCREEN();
                perror("파일을 열 수 없습니다.");
                return;
            }

            // 파일에 도서를 작성
            writeBookToFile(file, (BookData) { code, category, group, year, month, day });

            // 파일 닫기
            fclose(file);

            printf("도서가 성공적으로 추가되었습니다.\n");
            break;
        }
        else {
            // 비정상적인 코드인 경우 대체 코드를 생성하여 파일에 추가
            printf("잘못된 도서코드 형식입니다. 대체 코드를 생성하여 추가합니다.\n");

            char replacementCode[15];
            generateReplacementCode(replacementCode);

            FILE* file = fopen("library.csv", "a");
            if (file == NULL) {
                CLEAR_SCREEN();
                perror("파일을 열 수 없습니다.");
                return;
            }

            // 대체 코드를 파일에 작성
            fprintf(file, "%s\n", replacementCode);

            // 파일 닫기
            fclose(file);

            printf("추가된 대체코드 : %s\n", replacementCode);
            break;
        }
    }
}

// 도서를 데이터 배열에서 수정하는 함수
void modifyBook(BookData arr[], int dataSize) {
    CLEAR_SCREEN();
    long long int searchCode;
    long long int code;

    // 1. 값 입력
    printf("수정할 도서코드를 입력하세요: ");
    scanf("%lld", &searchCode);

    // 2. 도서코드 탐색
    int foundIndex = -1;
    for (int i = 0; i < dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    // 3. 도서코드가 없는 경우
    if (foundIndex == -1) {
        printf("해당 도서코드가 존재하지 않습니다.\n");

        char retry;
        printf("다시 입력하시겠습니까? [y/n]: ");
        scanf(" %c", &retry);

        // 4. 다시 입력할 것인지 확인
        if (retry == 'y') {
            modifyBook(arr, dataSize); // 재귀 호출을 통해 함수 재실행
        }
        else {
            printf("프로그램을 종료합니다.\n");
            return; // 종료
        }
    }
    else {
        char confirmModify;
        // 5. 수정 여부 다시 확인
        printf("도서코드 %lld를 수정하시겠습니까? [y/n]: ", searchCode);
        scanf(" %c", &confirmModify);

        if (confirmModify == 'y') {
            // 6. 수정할 값을 입력받음
            char newCode[15];
            do {
                printf("새로운 도서코드를 입력하세요: ");
                scanf("%s", newCode);

                // 7. 입력된 값이 유효한지 확인
                if (!isValidCode(newCode)) {
                    printf("유효하지 않은 도서코드입니다. 다시 입력하세요.\n");
                }
                else {
                    // 8. 유효한 경우, 날짜가 존재하는지 확인
                    code = atoll(newCode);
                    long long int category, group, year, month, day;
                    parseCode(code, &category, &group, &year, &month, &day);

                    if (!isValidDate(year, month, day)) {
                        printf("유효하지 않은 날짜입니다. 다시 입력하세요.\n");
                    }
                    else {
                        // 9. 날짜가 유효한 경우, 도서코드를 수정
                        arr[foundIndex].code = code;
                        printf("도서코드가 성공적으로 수정되었습니다.\n");

                        // 10. 파일 열어서 수정된 코드로 덮어씌우기
                        FILE* file = fopen("library.csv", "r");
                        if (file == NULL) {
                            perror("파일을 열 수 없습니다.");
                            return;
                        }

                        // 임시 파일 생성
                        FILE* tempFile = fopen("temp.csv", "w");
                        if (tempFile == NULL) {
                            perror("임시 파일을 생성할 수 없습니다.");
                            fclose(file);
                            return;
                        }

                        // 기존 파일을 읽어서 수정된 코드로 덮어씌우기
                        long long int currentCode;
                        while (fscanf(file, "%lld", &currentCode) == 1) {
                            if (currentCode == searchCode) {
                                // 수정된 코드로 교체
                                fprintf(tempFile, "%lld\n", code);
                            }
                            else {
                                // 기존 코드 그대로 유지
                                fprintf(tempFile, "%lld\n", currentCode);
                            }
                        }

                        // 파일 닫기
                        fclose(file);
                        fclose(tempFile);

                        // 수정이 완료된 임시 파일을 원본 파일로 복사
                        remove("library.csv"); // 원본 파일 삭제
                        rename("temp.csv", "library.csv"); // 임시 파일을 원본 파일로 이름 변경

                        break;
                    }
                }
            } while (true);
        }
        else {
            printf("도서코드 수정이 취소되었습니다.\n");
        }
    }
}

// 도서를 데이터 배열에서 삭제하는 함수
void deleteBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    long long int searchCode;

    // 1. 값 입력
    printf("삭제할 도서코드를 입력하세요: ");
    scanf("%lld", &searchCode);

    // 2. 도서코드 탐색
    int foundIndex = -1;
    for (int i = 0; i < *dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    // 3. 도서코드가 없는 경우
    if (foundIndex == -1) {
        printf("해당 도서코드가 존재하지 않습니다.\n");

        char retry;
        printf("다시 입력하시겠습니까? [y/n]: ");
        scanf(" %c", &retry);

        // 4. 다시 입력할 것인지 확인
        if (retry == 'y') {
            deleteBook(arr, dataSize); // 재귀 호출을 통해 함수 재실행
        }
        else {
            printf("프로그램을 종료합니다.\n");
            return; // 종료
        }
    }
    else {
        char confirmDelete;
        // 5. 삭제 여부 다시 확인
        printf("도서코드 %lld를 삭제하시겠습니까? [y/n]: ", searchCode);
        scanf(" %c", &confirmDelete);

        if (confirmDelete == 'y') {
            // 6. 삭제 수행
            for (int i = foundIndex; i < *dataSize - 1; i++) {
                arr[i] = arr[i + 1];
            }

            (*dataSize)--;
            printf("도서코드가 성공적으로 삭제되었습니다.\n");
        }
        else {
            printf("도서코드 삭제가 취소되었습니다.\n");
        }
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

int runMainMenu(BookData arr[], int dataSize) {
    int choice = -1;

    do {
        showMainMenu();
        printf("원하는 작업을 선택하세요: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // 도서코드 전체 리스트 출력
            printAllBooks(arr, dataSize);
            break;
        case 2:
            // 도서코드 검색
            searchBooks(arr, dataSize);
            break;
        case 3:
            // 도서코드 추가
            addBook(arr, &dataSize); // arr과 dataSize를 함수에 전달
            break;
        case 4:
            // 도서코드 수정
            modifyBook(arr, dataSize);
            break;
        case 5:
            // 도서코드 삭제
            deleteBook(arr, &dataSize); // arr과 dataSize를 함수에 전달
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
        parseCode(data[dataSize].code, &data[dataSize].category, &data[dataSize].group, &data[dataSize].year, &data[dataSize].month, &data[dataSize].day);
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
        continueProgram = runMainMenu(data, dataSize);
    } while (continueProgram != 0);
    
    // 할당된 메모리 해제
    free(data);
    CLEAR_SCREEN();
    printf("프로그램이 종료되었습니다.");
    return 0;
}