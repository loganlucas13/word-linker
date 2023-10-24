#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int countWordsOfLength(char* filename, int wordSize) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    int count = 0;
    char line[100];
    while (fgets(line, 100, file)) {
        strtok(line, "\n");
        if (strlen(line) == wordSize) {
            count++;
        }
    }
    fclose(file);

    return count;
}

void testCountWordsOfLength() {
    printf("  Testing countWordsOfLength()\n");
    char filename[] = "simple3.txt";
    
    int result = countWordsOfLength(filename, 3);
    printf("   Expected result: 26\n");
    printf("   Actual result: %d\n", result);

    strcpy(filename, "simple4.txt");

    result = countWordsOfLength(filename, 4);
    printf("   Expected result: 30\n");
    printf("   Actual result: %d\n", result);

    strcpy(filename, "simple5.txt");

    result = countWordsOfLength(filename, 5);
    printf("   Expected result: 10\n");
    printf("   Actual result: %d\n", result);

}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    int count = 0;
    char line[100];
    while (fgets(line, 100, file)) {
        strtok(line, "\n"); // removes newline character
        printf("length: %ld\n", strlen(line));
        if (strlen(line) == wordSize) {
            line[wordSize] = '\0'; // adds null character to end
            strcpy(words[count], line);
            printf("%s\n", words[count]);
            count++;
        }
    }
    fclose(file);

    printf("expected count: %d\n", numWords);
    printf("actual count: %d\n", count);
    if (count != numWords) {
        return false;
    }
    return true;
}

void testBuildWordArray() {
    printf("  Testing buildWordArray()\n");
    char filename[] = "simple3.txt";
    int numWords = 26;
    int wordSize = 3;
    bool passed;

    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; i++) {
        words[i] = (char*)malloc(wordSize*sizeof(char));
    }

    passed = buildWordArray(filename, words, numWords, wordSize);
    
    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

int main() {
    printf("Running Tests:\n");

    testCountWordsOfLength();

    testBuildWordArray();


    return 0;
}