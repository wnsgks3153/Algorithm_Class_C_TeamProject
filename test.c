#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

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
    printf("%d. '%03lld%03lld%04lld%02lld%02lld'\n", resultIndex, book.category, book.code, book.year, book.month, book.day);
}


// 검색 함수
void searchBooks(BookData arr[], int dataSize, long long int category, long long int code, long long int year, long long int month, long long int day) {
    bool found = false;
    int resultIndex = 0;

    CLEAR_SCREEN();

    printf("[ 검색 결과 ]\n");

    // 검색된 도서코드를 출력
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
        printf("일치하는 도서코드가 없습니다.\n");
    }
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

    // 검색 기능
    long long int searchCategory = -1;  // -1은 해당 항목을 무시하고 검색
    long long int searchCode = -1;
    long long int searchYear = -1;
    long long int searchMonth = -1;
    long long int searchDay = -1;

    // 사용자 입력을 받아 검색 조건 설정
    searchCategory = getSearchInput("검색하려고 하는 도서분류 값을 입력하세요 (-1은 무시): ");
    searchCode = getSearchInput("검색하려고 하는 분류기호 값을 입력하세요 (-1은 무시): ");
    searchYear = getSearchInput("검색하려고 하는 해당 년도를 입력하세요 (-1은 무시): ");
    searchMonth = getSearchInput("검색하려고 하는 월을 입력하세요 (-1은 무시): ");
    searchDay = getSearchInput("검색하려고 하는 해당일을 입력하세요 (-1은 무시): ");

    // 검색 함수 호출
    searchBooks(data, dataSize, searchCategory, searchCode, searchYear, searchMonth, searchDay);

    // 할당된 메모리 해제
    free(data);

    return 0;
}