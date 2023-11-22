#define _CRT_SECURE_NO_WARNINGS
// 페이지에 표시할 도서의 수
#define BOOKS_PER_PAGE 8000

//명령 프롬포트 화면 지우는 명령어 정의
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

// 도서코드 항목별로 구분한 구조체 생성
typedef struct {
    long long int code;       // 도서코드전체(14자리)
    long long int category;   // 도서분류
    long long int group;      // 분류기호
    long long int year;       // 연도
    long long int month;      // 월
    long long int day;        // 일
} BookData;

// 출력 시 중앙에서 출력하는 명령어 정의
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

// 퀵 정렬 함수에서 사용하는 SWAP 함수
void swap(BookData* a, BookData* b) {
    BookData temp = *a;
    *a = *b;
    *b = temp;
}

// 도서코드 데이터를 불러와서 데이터를 순차적으로 정렬하기 위한 퀵정렬 함수
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

// 도서코드의 각 자릿수를 분리하여 구조체 멤버에 저장
void parseCode(long long int code, long long int* category, long long int* group, long long int* year, long long int* month, long long int* day) {
    *category = code / 100000000000;          // 도서코드를 1000억으로 나눠서 14자리 도서코드의 앞에서 1~3번째 위치의 코드를 도서분류 항목에 저장
    *group = (code / 100000000) % 100000;     // 도서코드를 1억으로 나눈 값을 10만으로 나누어 14자리 도서코드의 앞에서 1~6번째 위치의 코드를 분류기호 항목에 저장
    *year = (code % 100000000) / 10000;       // 도서코드를 1억으로 나눈 나머지를 1만으로 나누어 14자리 도서코드의 앞에서 7~10번째 위치의 코드를 년도 항목에 저장
    *month = (code % 10000) / 100;            // 도서코드를 1만으로 나눈 나머지를 백으로 나누어 14자리 도서코드의 앞에서 11~12번째 위치의 코드를 년도 항목에 저장
    *day = code % 100;                        // 도서코드를 100으로 나눈 나머지를 14자리 도서코드의 앞에서 13~14번째 위치의 코드를 년도 항목에 저장
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

// 도서코드 검색 함수에서 항목 별로 검색 입력을 받는 함수
long long int getSearchInput(const char* prompt) {
    long long int input;
    while (true) { // 올바른 값이 입력될 때까지 반복
        printf("%s", prompt);
        if (scanf("%lld", &input) == 1) {
            break; // 올바른 값이 입력되었다면 함수를 종료
        }
        else {
            printf("\t\t\t\t\t  잘못된 입력입니다. 다시 입력하세요.\n"); //
            while (getchar() != '\n');  // 잘못된 값이 입력된 경우 경고문을 출력하고 입력 받았던 값의 버퍼를 초기화
        }
    }
    return input; // 입력된 값을 다시 반환
}

// 각 숫자가 입력에 존재하는지 확인하는 함수
int isDigitPresent(char* input, char digit) {
    for (int i = 0; i < strlen(input); i++) { // 한글자씩 숫자 입력이 존재하는지 확인
        if (input[i] == digit) {
            return 1; // 숫자가 존재함
        }
    }
    return 0; // 숫자가 존재하지 않음
}

// 각 숫자에 따라 사용자에게 검색 기준을 입력 받는 함수
void promptUserForSearchCriteria(char* input, long long int* searchCategory, long long int* searchGroup, long long int* searchYear, long long int* searchMonth, long long int* searchDay) {
    if (isDigitPresent(input, '1')) { // 1을 선택한 경우 검색할 도서분류 값을 입력 받음
        *searchCategory = getSearchInput("\t\t\t\t\t검색할 도서 분류 값을 입력하세요 : ");
        while (*searchCategory < 0) {
            printf("\t\t\t\t\t올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchCategory = getSearchInput("\t\t\t\t\t검색할 도서 분류 값을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '2')) { // 2을 선택한 경우 검색할 분류기호 값을 입력 받음
        *searchGroup = getSearchInput("\t\t\t\t\t검색할 분류 기호 값을 입력하세요 : ");
        while (*searchGroup < 0) {
            printf("\t\t\t\t\t올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchGroup = getSearchInput("\t\t\t\t\t검색할 분류 기호 값을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '3')) { // 3을 선택한 경우 검색할 해당 년도 값을 입력 받음
        *searchYear = getSearchInput("\t\t\t\t\t검색할 해당 년도 값을 입력하세요 : ");
        while (*searchYear < 0) {
            printf("\t\t\t\t\t올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchYear = getSearchInput("\t\t\t\t\t  검색할 해당 년도 값을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '4')) { // 4을 선택한 경우 검색할 해당 달의 값을 입력 받음
        *searchMonth = getSearchInput("\t\t\t\t\t검색할 해당 달의 값을 입력하세요 : ");
        while (*searchMonth < 0) {
            printf("\t\t\t\t\t올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchMonth = getSearchInput("\t\t\t\t\t검색할 해당 달의 값을 입력하세요 : ");
        }
    }
    if (isDigitPresent(input, '5')) { // 5을 선택한 경우 검색할 해당 일의 값을 입력 받음
        *searchDay = getSearchInput("\t\t\t\t\t검색할 해당 일의 값을 입력하세요 : ");
        while (*searchDay < 0) {
            printf("\t\t\t\t\t올바르지 않은 값입니다. 다시 입력하세요.\n");
            *searchDay = getSearchInput("\t\t\t\t\t검색할 해당 일의 값을 입력하세요 : ");
        }
    }
}


// 페이지 출력 함수
void printPage(BookData arr[], int dataSize, int startIdx, int endIdx) {
    for (int i = startIdx; i <= endIdx; i++) {
        printf("\t\t\t\t\t\t%d. %14lld\n", i + 1, arr[i].code);
    }
}

// 전체 도서 코드 출력 함수
void printAllBooks(BookData arr[], int dataSize) {
    CLEAR_SCREEN();

    int totalPages = (dataSize + BOOKS_PER_PAGE - 1) / BOOKS_PER_PAGE; // 올림을 이용하여 페이지 수 계산
    int currentPage = 1;
    int startIdx = 0;
    int endIdx = (dataSize < BOOKS_PER_PAGE) ? dataSize - 1 : BOOKS_PER_PAGE - 1; // 마지막 페이지를 계산

    while (true) {
        printf("\t\t\t\t\t\t==== 도서 코드 목록 ====\n\n");
        printPage(arr, dataSize, startIdx, endIdx);
        printf("\n");

        // 다음 페이지 또는 이전 페이지로 이동할지 여부 묻기
        char moveKey;

        if (currentPage == 1) { // 첫 페이지인 경우 이전 페이지로 이동하는 옵션 비활성화
            printf("\t\t\t\t\t\t  [ 페이지 %d / %d ]\n\n", currentPage, totalPages);
            printf("\t\t=======================================================================================\n");
            printf("\t\t||   이전 페이지로 이동 불가   ||  다음 페이지로 이동[n]  ||         종료[q]         ||\n");
            printf("\t\t=======================================================================================\n");
            centerText("선택 : ");
            scanf(" %c", &moveKey);
            if (moveKey == 'n' && currentPage < totalPages) {
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
        else if (currentPage == totalPages) { // 마지막 페이지인 경우 다음 페이지로 이동하는 옵션 비활성화
            printf("\t\t\t\t\t\t  [ 페이지 %d / %d ]\n\n", currentPage, totalPages);
            printf("\t\t=======================================================================================\n");
            printf("\t\t||   이전 페이지로 이동[p]   ||  다음 페이지로 이동 불가  ||         종료[q]         ||\n");
            printf("\t\t=======================================================================================\n");
            centerText("선택 : ");
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
        else { // 첫 페이지, 마지막 페이지가 아닌 경우 이전페이지와 다음페이지 선택 가능
            printf("\t\t\t\t\t\t  [ 페이지 %d / %d ]\n\n", currentPage, totalPages);
            printf("\t\t=======================================================================================\n");
            printf("\t\t||   이전 페이지로 이동[p]   ||   다음 페이지로 이동[n]   ||         종료[q]         ||\n");
            printf("\t\t=======================================================================================\n");
            centerText("선택 : ");
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

// 도서코드를 검색하는 함수
void searchBooks(BookData arr[], int dataSize) {

    CLEAR_SCREEN();

    long long int searchCategory;
    long long int searchGroup;
    long long int searchYear;
    long long int searchMonth;
    long long int searchDay;

    printf("\n");
    centerText("           [ 도서 코드 검색 ]           \n");
    centerText("========================================\n");
    centerText("||      검색할 항목을 선택하세요      ||\n");
    centerText("========================================\n");
    centerText("||       [ 항목 선택방법 예시 ]       ||\n");
    centerText("||                                    ||\n");
    centerText("||     111,314,412,123,12345,등등     ||\n");
    centerText("||     1~5 사이의 값만 입력한다면     ||\n");
    centerText("||      어떻게 입력해도 상관없음      ||\n");
    centerText("========================================\n");
    centerText("||                                    ||\n");
    centerText("||             1.도서분류             ||\n");
    centerText("||                                    ||\n");
    centerText("||             2.분류기호             ||\n");
    centerText("||                                    ||\n");
    centerText("||             3.년도                 ||\n");
    centerText("||                                    ||\n");
    centerText("||             4.월                   ||\n");
    centerText("||                                    ||\n");
    centerText("||             5.일                   ||\n");
    centerText("||                                    ||\n");
    centerText("========================================\n");

    char input[20];

    // 검색할 항목을 
    while (true) { // 값이 올바르게 입력될 때 까지 반복
        centerText("검색할 항목을 선택하세요: ");
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

        if (validInput) { // 올바른 값이 입력되면 반복문 종료
            break;
        }
        else { 
            centerText("잘못된 입력입니다. 다시 입력하세요.\n");
            while (getchar() != '\n');  // 올바르지 않은 값이 입력된 경우 다시 입력을 받음
        }
    }

    // 입력된 값 출력, 어떤 항목을 선택했는지 출력
    printf("\t\t\t\t\t\t  입력된 값: %s\n", input);

    // 중복된 숫자 처리
    removeDuplicateDigits(input);

    // 선택한 항목별로 다시 검색하고자 하는 값을 입력하여 탐색
    promptUserForSearchCriteria(input, &searchCategory, &searchGroup, &searchYear, &searchMonth, &searchDay);

    // 실제 검색 수행
    bool found = false;
    int resultIndex = 0;

    // 검색 결과 출력
    CLEAR_SCREEN();
    centerText("========================================\n");
    centerText("[ 검색 결과 ]\n");
    centerText("========================================\n\n");
    int n = 0;

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
            printf("\t\t\t\t\t\t%d. %03lld%03lld%04lld%02lld%02lld\n", resultIndex, arr[i].category, arr[i].group, arr[i].year, arr[i].month, arr[i].day);
            n++;
            found = true;
        }
    }
    printf("\n");
    centerText("========================================\n");

    if (!found) { // 검색결과 일치하는 도서코드가 없다면 일치하는 코드가 없음을 출력
        centerText("일치하는 도서코드가 없습니다.\n");
        centerText("========================================\n");
        printf("\n");
    }
    else { // 검색결과 일치하는 도서코드가 있으면 n개의 도서코드가 검색되었음을 출력
        printf("\t\t\t\t\t총 %d 개의 도서코드가 검색되었습니다.\n",n);
        centerText("========================================\n");
        printf("\n");
    }

}

// 파일에 도서 코드를 추가하는 함수
void writeBookToFile(FILE* file, BookData book) {
    fprintf(file, "%03lld%03lld%04lld%02lld%02lld\n", book.category, book.group, book.year, book.month, book.day);
}

// 비정상적인 값을 입력한 경우 대체코드를 삽입하기 위해 대체코드를 생성
void generateReplacementCode(char* replacementCode) {
    // localtime 함수로 현재 날짜를 불러온다.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // 'NSC(현재 날짜)' 형태로 대체 코드 생성
    sprintf(replacementCode, "NSC%d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// 입력된 도서 코드가 유효한 값인지 확인하는 함수
bool isValidCode(const char* input) {
    // 길이가 14자리인지 확인
    if (strlen(input) != 14) {
        return false; // 14자리보다 크거나 작으면 적절하지 않은 값이라 판단
    }

    // 모든 문자가 숫자인지 확인
    for (int i = 0; i < 14; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return false; // 입력된 값에 0이상의 정수값이 아닌 다른 값이 입력된 적절하지 않은 값이라 판단
        }
    }

    return true; // 모든 검사를 통과했다면 올바른 값이라 판단
}

// 날짜가 유효한지 확인하는 함수
bool isValidDate(long long int year, long long int month, long long int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    } // 년도값이 0보다 작거나, 월 값이 1~12월 사이의 값이 아닌경우, 일 값이 1보다 작은 음수인경우 False를 반환

    // 각 월별 일수를 확인하여 유효성 검사
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        // 윤년인 경우 2월은 29일까지 유효
        daysInMonth[2] = 29;
    }

    return day <= daysInMonth[month]; // 값을 다시 반환
}

// 새로운 도서코드를 library.csv 파일에 추가하는 함수
void addBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    char inputCode[15];  // 코드를 입력받을 문자열, 코드는 14자리이며 널 종료 문자를 고려해 15로 설정
    long long int code;

    // 1단계: 문자열로 추가할 도서 코드 입력
    while (true) {
        printf("\n");
        centerText("[ 도서 코드 추가 ]\n");
        centerText("========================================\n");
        centerText("||   추가하실 도서코드를 입력하세요   ||\n");
        centerText("========================================\n");
        printf("\n\t\t\t\t\t입력 : ");

        scanf("%s", inputCode);

        // 2단계: 입력이 문자열 또는 0 이상의 양의 정수인지 확인
        if (isValidCode(inputCode)) {
            // 3단계: 입력 가능한 코드라 판단되면 문자열을 삽입가능하게 long long int로 변환
            code = atoll(inputCode);

            // 4단계: 유효한 코드라 판단된 코드에서 날짜가 존재하는 날짜인지 판단
            long long int category, group, year, month, day;
            parseCode(code, &category, &group, &year, &month, &day);

            // 5-1단계: 날짜가 유효하지 않은 경우 다시 입력 받음
            if (!isValidDate(year, month, day)) {
                CLEAR_SCREEN();
                printf("유효하지 않은 날짜입니다. 다시 입력하세요.\n");
                continue;
            }

            // 5단계 코드가 유효한 경우, 도서코드를 파일에 추가
            FILE* file = fopen("library.csv", "a");
            if (file == NULL) {
                CLEAR_SCREEN();
                perror("파일을 열 수 없습니다.");
                return;
            }

            // 6단계: library.csv 파일에 도서코드를 삽입
            writeBookToFile(file, (BookData) { code, category, group, year, month, day });

            // 파일 닫기
            fclose(file);

            printf("\n");
            centerText("========================================\n");
            centerText("도서가 성공적으로 추가되었습니다.\n");
            centerText("========================================\n");
            break;
        }
        else {
            // 3-1단계: 비정상적인 코드인 경우 대체 코드를 생성하여 파일에 추가
            printf("\n");
            centerText("잘못된 도서코드 형식입니다. 대체 코드를 생성하여 추가합니다.\n\n");

            char replacementCode[15];
            generateReplacementCode(replacementCode); // 3-2단계: 비정상적인 코드 대신 삽입할 대체코드 생성

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

            printf("\t\t\t\t\t추가된 대체코드 : %s\n\n", replacementCode);
            break;
        }
    }
}

// 도서코드를 데이터 배열에서 수정하는 함수
void modifyBook(BookData arr[], int dataSize) {
    CLEAR_SCREEN();
    char searchCode[20];
    long long int code;
    char confirmModify;
    char newCode[20];
    long long int intNewCode;

    // 1단계: 수정하고자 하는 코드가 파일 내에 있는지 탐색
    printf("\n");
    centerText("[ 도서 코드 수정 ]\n");
    centerText("========================================\n");
    centerText("||   수정하실 도서코드를 입력하세요   ||\n");
    centerText("========================================\n");

    while (true) { // 2단계: 올바른 값이 입력될 때까지 while문 내의 과정을 반복
        printf("\n\t\t\t\t\t입력 : ");

        scanf("%s", searchCode);

        // 2단계: 입력된 값이 유효한 코드인지 판단
        if (isValidCode(searchCode)) { // 3단계: 유효한 코드라 판단되는 경우 실행
            // 3단계: 도서코드 값을 검사하고 코드를 삽입할 수 있게 문자열을 long long int로 변환
            code = atoll(searchCode);

            // 4단계: 유효한 코드라 판단된 코드에서 날짜가 존재하는 날짜인지 판단
            long long int category, group, year, month, day;
            parseCode(code, &category, &group, &year, &month, &day);

            // 4-1단계: 날짜가 유효하지 않은 경우 다시 입력 받음
            if (!isValidDate(year, month, day)) {
                CLEAR_SCREEN();
                printf("유효하지 않은 날짜입니다. 다시 입력하세요.\n");
                continue;  // 날짜가 유효하지 않으면 반복문 처음으로 돌아감
            }
            printf("\n");
            centerText("동일한 도서코드가 발견되었습니다.\n");
            break;
        } 
        else { // 2-1단계: 올바르지 않은 값이라 판단되는경우 다시 입력을 받음
            printf("\n");
            centerText("잘못된 도서코드 형식입니다. 다시 입력하세요.\n\n");
            continue;  // 잘못된 도서코드 형식이면 반복문 처음으로 돌아감
        }
    }

    // 5단계: 모든 검사에서 적합한 코드라 판단되면 파일 내의 일치하는 값을 검색, 찾는다면 그 값의 인덱스 위치 값을 저장
    int foundIndex = -1;
    for (int i = 0; i < dataSize; i++) {
        if (arr[i].code == code) {
            foundIndex = i;
            break;
        }
    }


    // 6-1단계: 파일 내에서 찾고자하는 코드가 없는 경우
    if (foundIndex == -1) {
        printf("\n");
        centerText("해당 도서코드가 존재하지 않습니다.\n");

        char retry;
        printf("\n");
        centerText("다시 입력하시겠습니까? [y/n]: ");
        scanf(" %c", &retry);

        // 6-2단계: 다시 입력할 것인지 확인
        if (retry == 'y') {            // 다시 입력을 선택하는 경우
            modifyBook(arr, dataSize); // 재귀 호출을 통해 함수 재실행
        }
        else {
            printf("프로그램을 종료합니다.\n");
            return; // 종료
        }
    }
    else { 
        do { // 6단계: 파일 내에서 일치하는 값을 찾은 경우 올바른 도서코드를 입력할 때까지 반복
            printf("\n");
            printf("\n\t\t\t\t    새로운 도서코드를 입력하세요: ");
            scanf("%s", newCode); // 7단계: 파일 내에 수정할 도서코드를 어떻게 바꿀지 입력

            // 8단계: 수정된 도서코드 값이 사용 가능한지 확인
            if (!isValidCode(newCode)) { // 9-1단계: 사용불가능한 도서코드라 판단되는경우 다시 입력을 받음
                printf("\n");
                centerText("유효하지 않은 도서코드입니다. 다시 입력하세요.\n");
            }
            else {
                // 9단계: 사용가능한 코드라 판단되면, 입력된 날짜가 실제로 존재하는지 확인
                intNewCode = atoll(newCode);
                long long int category, group, year, month, day;
                parseCode(intNewCode, &category, &group, &year, &month, &day);

                if (!isValidDate(year, month, day)) { // 10-1단계: 날짜가 존재하지 않는다면 다시 입력을 받음
                    printf("\n");
                    centerText("유효하지 않은 날짜입니다. 다시 입력하세요.\n");
                }
                else { // 10단계: 날짜가 존재한다면 도서코드를 삽입
                    printf("\n\t\t\t    도서코드 %lld를 %lld로 수정하시겠습니까? [y/n]: ", code, intNewCode);
                    scanf(" %c", &confirmModify);

                    if (confirmModify == 'y') {
                        arr[foundIndex].code = intNewCode;
                        printf("\n");
                        centerText("========================================\n");
                        centerText("||도서코드가 성공적으로 수정되었습니다||\n");
                        centerText("========================================\n");

                        // 11단계: 파일 열어서 수정된 코드로 덮어씌우기
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
                            if (currentCode == code) {
                                // 수정된 코드로 교체
                                fprintf(tempFile, "%lld\n", intNewCode);
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
                    else {
                        printf("\n");
                        centerText("========================================\n");
                        centerText("||  도서코드 수정이 취소 되었습니다.  ||\n");
                        centerText("========================================\n");
                        break;
                    }
                }
            }
        } while (true);
    }
}

// 도서코드를 library.csv에서 삭제하는 함수
void deleteBook(BookData arr[], int* dataSize) {
    CLEAR_SCREEN();
    long long int searchCode;

    // 1단계: 삭제할 도서코드를 입력
    printf("\n");
    centerText("[ 도서 코드 삭제 ]\n");
    centerText("========================================\n");
    centerText("||   삭제하실 도서코드를 입력하세요   ||\n");
    centerText("========================================\n");
    printf("\n\t\t\t\t\t입력 : ");
    scanf("%lld", &searchCode);

    // 2단계: 파일 내에 도서코드가 있는지 검색
    int foundIndex = -1;
    for (int i = 0; i < *dataSize; i++) {
        if (arr[i].code == searchCode) {
            foundIndex = i;
            break;
        }
    }

    // 3-1단계: 도서코드가 없는 경우 실행
    if (foundIndex == -1) {
        printf("\n");
        centerText("해당 도서코드가 존재하지 않습니다.\n");

        char retry;
        printf("\n");
        centerText("다시 입력하시겠습니까? [y/n]: ");
        scanf(" %c", &retry);

        // 3-2단계: 다시 입력할 것인지 확인
        if (retry == 'y') {
            deleteBook(arr, dataSize); // 재귀 호출을 통해 함수 재실행
        }
        else {
            printf("\n");
            centerText("========================================\n");
            centerText("||       프로그램을 종료합니다.       ||\n");
            centerText("========================================\n");
            return; // 종료
        }
    }
    else { // 3단계: 파일 내에 동일한 도서코드가 있는경우 실행
        char confirmDelete;
        // 4단계: 삭제 여부 다시 확인
        printf("\n");
        printf("\n\t\t\t\t도서코드 %lld를 삭제하시겠습니까? [y/n]: ", searchCode);
        scanf(" %c", &confirmDelete);

        if (confirmDelete == 'y') {
            // 5단계: 삭제 수행
            for (int i = foundIndex; i < *dataSize - 1; i++) {
                arr[i] = arr[i + 1];
            }

            (*dataSize)--;
            printf("\n");
            centerText("========================================\n");
            centerText("||도서코드가 성공적으로 삭제되었습니다||\n");
            centerText("========================================\n");

            // 6단계: 파일 열어서 삭제된 코드를 'library.csv' 파일에서 삭제
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

            // 기존 파일을 읽어서 삭제된 코드를 'library.csv' 파일에서 삭제
            long long int currentCode;
            while (fscanf(file, "%lld", &currentCode) == 1) {
                if (currentCode != searchCode) {
                    // 삭제되지 않은 코드만 임시 파일에 쓰기
                    fprintf(tempFile, "%lld\n", currentCode);
                }
            }

            // 파일 닫기
            fclose(file);
            fclose(tempFile);

            // 삭제가 완료된 임시 파일을 원본 파일로 복사
            remove("library.csv"); // 원본 파일 삭제
            rename("temp.csv", "library.csv"); // 임시 파일을 원본 파일로 이름 변경
        }
        else {
            printf("\n");
            centerText("========================================\n");
            centerText("||   도서코드 삭제가 취소되었습니다   ||\n");
            centerText("========================================\n");
        }
    }
}

// 메인 메뉴 UI 구현 함수
void showMainMenu() {
    printf("\n\n");
    centerText("========== 알고리즘 설계 C반 팀프로젝트 3조 ==========\n");
    centerText("||   [ 김동찬 ]       [ 이준한 ]       [ 이창호 ]   ||\n");
    centerText("======================================================\n");
    centerText("||                                                  ||\n");
    centerText("||              [ 도서 관리 프로그램 ]              ||\n");
    centerText("||                                                  ||\n");
    centerText("======================================================\n");
    centerText("||                                                  ||\n");
    centerText("||                 1. 도서코드 출력                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 2. 도서코드 검색                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 3. 도서코드 추가                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 4. 도서코드 수정                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 5. 도서코드 삭제                 ||\n");
    centerText("||                                                  ||\n");
    centerText("||                 0. 프로그램 종료                 ||\n");
    centerText("||                                                  ||\n");
    centerText("======================================================\n");
    centerText("======================================================\n");
    printf("\n");
}

// 메인메뉴 실행 함수
int runMainMenu(BookData arr[], int dataSize) {
    int choice = -1;

    do {
        showMainMenu();
        centerText("원하는 작업을 선택하세요: ");

        if (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            // 정수가 아니거나 0에서 5 사이의 값이 아닌 경우
            CLEAR_SCREEN();
            printf("잘못된 입력입니다. 다시 입력하세요.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            choice = -1; // 다시 입력 받기 위해 choice를 -1로 설정
            continue; // 다시 반복문의 처음으로 이동
        }

        // 사용할 기능을 선택
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
            addBook(arr, &dataSize);
            break;
        case 4:
            // 도서코드 수정
            modifyBook(arr, dataSize);
            break;
        case 5:
            // 도서코드 삭제
            deleteBook(arr, &dataSize);
            break;
        case 0:
            // 프로그램 종료
            printf("프로그램을 종료합니다.\n");
            break;
        default:
            printf("잘못된 입력입니다. 다시 입력하세요.\n");
        }

        if (choice != 0) {
            char returnToMain;
            printf("\n");
            printf("메인 화면으로 돌아가시겠습니까? [y/n] : ");
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

int main() { // 메인 함수
    // library.csv 파일 열기
    FILE* file = fopen("library.csv", "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 1;
    }

    // 데이터 배열 초기 크기 설정
    int initialSize = 30000;

    // 구조체 데이터 배열 동적 할당
    BookData* data = malloc(initialSize * sizeof(BookData));
    if (data == NULL) {
        perror("메모리를 할당할 수 없습니다.");
        fclose(file);
        return 1;
    }

    // 데이터 개수와 현재 배열 크기 초기화
    int dataSize = 0;
    int currentSize = initialSize;

    // 파일에서 데이터 읽어와 구조체의 각 항목 별 저장
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

    // 파일을 다 사용했다면 닫기
    fclose(file);

    // 읽어온 데이터를 퀵 정렬을 통해 순서대로 다시 정렬
    quickSort(data, 0, dataSize - 1);

    int continueProgram;

    do { // 메인 메뉴를 계속 보여주게 설정
        continueProgram = runMainMenu(data, dataSize);
    } while (continueProgram != 0);

    // 할당된 메모리 해제
    free(data);
    CLEAR_SCREEN();
    printf("프로그램이 종료되었습니다.");
    return 0;
}