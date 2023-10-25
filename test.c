#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next;
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next;
} LadderNode;

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
        if (strlen(line) == wordSize) {
            line[wordSize] = '\0'; // adds null character to end
            strcpy(words[count], line);
            count++;
        }
    }
    fclose(file);

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
        printf("   %d: %s\n", i, words[i]);
    }

    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

int findWord(char** words, char* aWord, int loInd, int hiInd) {
    while (loInd <= hiInd) {
        int mid = (loInd + hiInd) / 2;

        // aWord found at mid
        if (strcmp(aWord, words[mid]) == 0) {
            return mid;
        }

        if (strcmp(aWord, words[mid]) > 0) {
            loInd = mid+1; // ignore left half
        }
        else {
            hiInd = mid-1; // ignore right half
        }
    }
    return -1; // not found
}

void testFindWord() {
    printf("  Testing findWord()\n");
    char filename[] = "simple3.txt";
    int numWords = 26;
    int wordSize = 3;
    bool passed;

    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; i++) {
        words[i] = (char*)malloc(wordSize*sizeof(char));
    }

    passed = buildWordArray(filename, words, numWords, wordSize);
    printf("   Searching for 'aim'\n");
    printf("    expected result: 0\n");
    printf("    actual result: %d\n", findWord(words, "aim", 0, numWords-1));

    passed = buildWordArray(filename, words, numWords, wordSize);
    printf("   Searching for 'tye'\n");
    printf("    expected result: 25\n");
    printf("    actual result: %d\n", findWord(words, "tye", 0, numWords-1));

    passed = buildWordArray(filename, words, numWords, wordSize);
    printf("   Searching for 'lip'\n");
    printf("    expected result: 14\n");
    printf("    actual result: %d\n", findWord(words, "lip", 0, numWords-1));

    passed = buildWordArray(filename, words, numWords, wordSize);
    printf("   Searching for 'log'\n");
    printf("    expected result: 15\n");
    printf("    actual result: %d\n", findWord(words, "log", 0, numWords-1));

    passed = buildWordArray(filename, words, numWords, wordSize);
    printf("   Searching for 'bum'\n");
    printf("    expected result: 3\n");
    printf("    actual result: %d\n", findWord(words, "bum", 0, numWords-1));

    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("   %s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

void insertWordAtFront(WordNode** ladder, char* newWord) {
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    newNode->myWord = newWord;
    newNode->next = (*ladder);
    (*ladder) = newNode;
}

WordNode* copyLadder(WordNode* ladder) {
    if (ladder == NULL)  { // base case
        return NULL;
    }

    WordNode* newLadder = (WordNode*)malloc(sizeof(WordNode));
    newLadder->myWord = ladder->myWord;

    newLadder->next = copyLadder(ladder->next); // recursive step

    return newLadder;
}

void testInsertAndCopy() {
    printf("  Testing insertWordAtFront()\n");

    char filename[] = "simple3.txt";
    int numWords = 26;
    int wordSize = 3;
    bool passed;

    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; i++) {
        words[i] = (char*)malloc(wordSize*sizeof(char));
    }
    passed = buildWordArray(filename, words, numWords, wordSize);

    WordNode* ladder = (WordNode*)malloc(numWords*sizeof(WordNode));
    ladder = NULL;
    for (int i = 0; i < numWords; i++) {
        insertWordAtFront(&ladder, words[i]);
    }

    printLadder(ladder);

    printf("  Testing copyLadder()\n");
    WordNode* copy = copyLadder(ladder);

    printLadder(copy);
}

int main() {
    printf("Running Tests:\n");

    testCountWordsOfLength();

    testBuildWordArray();

    testFindWord();

    testInsertAndCopy();

    return 0;
}