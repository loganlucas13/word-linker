// Logan Lucas
// CS 211
// Fall 2023
// Project 5
// Developed using VSCode and Advanced Zybooks IDE

// This program finds the shortest word ladder between two words; utilizes linked lists

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

// @brief: reads words from file and counts number of words of length wordSize
// @param: filename, wordSize
// @return: number of words of length wordSize
int countWordsOfLength(char* filename, int wordSize) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    int count = 0;
    char line[100];
    while (fgets(line, 100, file)) {
        strtok(line, "\n"); // removes newline character
        if (strlen(line) == wordSize) {
            count++;
        }
    }
    fclose(file);

    return count;
}

// @brief: builds word array (passed by ptr) from file
// @param: filename, word array (ptr), expected size of wordArray, size of word
// @return: bool; true if successful, false if file cannot be read OR incorrect count
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

// @brief: finds aWord within words array using binary search
// @param: words (c-string array), aWord, lower bound, upper bound
// @return: index of word in words; -1 if not found
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

// @brief: frees all dynamically allocated memory for words array
// @param: words (ptr), size of words
// @return: none
void freeWords(char** words, int numWords) {
    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

// @brief: inserts word at front of word ladder
// @param: word ladder (linked list), word to add
// @return: none
void insertWordAtFront(WordNode** ladder, char* newWord) {
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    newNode->myWord = newWord;
    newNode->next = (*ladder);
    (*ladder) = newNode;
}

// @brief: finds total height of ladder
// @param: word ladder (linked list)
// @return: height
int getLadderHeight(WordNode* ladder) {
    int count = 0;
    WordNode* curr = ladder;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

// @brief: copies word ladder to a new WordNode* recursively
// @param: word ladder (linked list)
// @return: copied ladder
WordNode* copyLadder(WordNode* ladder) {
    if (ladder == NULL)  { // base case
        return NULL;
    }

    WordNode* newLadder = (WordNode*)malloc(sizeof(WordNode));
    newLadder->myWord = ladder->myWord;

    newLadder->next = copyLadder(ladder->next); // recursive step

    return newLadder;
}

// @brief: frees all dynamically allocated memory for a word ladder
// @param: word ladder (linked list)
// @return: none
void freeLadder(WordNode* ladder) {
    WordNode* curr = ladder;
    while (curr != NULL) {
        WordNode* temp = curr->next;
        free(curr);
        curr = temp;
    }
}

// @brief: inserts a word ladder at the end of a LadderNode list
// @param: LadderNode list (ptr), word ladder to add
// @return: none
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    LadderNode* newNode = (LadderNode*)malloc(sizeof(LadderNode));
    newNode->topWord = newLadder;
    newNode->next = NULL;

    if (*list == NULL) {
        (*list) = newNode;
        return;
    }

    LadderNode* curr = *list;
    while (curr) {
        if (curr->next == NULL) {
            curr->next = newNode;
            break;
        }
        curr = curr->next;
    }

}

// @brief: deallocates front LadderNode memory from list
// @param: LadderNode list (ptr)
// @return: front WordNode
WordNode* popLadderFromFront(LadderNode** list) {
    if (*list == NULL) {
        return NULL;
    }

    WordNode* frontNode = (*list)->topWord;
    LadderNode* newFront = (*list)->next;

    free(*list);
    *list = newFront;

    return frontNode;
}

// @brief: frees all dynamically allocated memory for a ladder list
// @param: LadderNode list
// @return: none
void freeLadderList(LadderNode* myList) {
    LadderNode* curr = myList;
    while (curr != NULL) {
        LadderNode* temp = curr->next;
        freeLadder(curr->topWord);
        free(curr);
        curr = temp;
    }
}

// @brief: counts total number of char differences between word1 and word2
// @param: words to be compared, size of words
// @return: total number of differences
int countDifferences(char* word1, char* word2, int wordSize) {
    int count = 0;
    for (int i = 0; i < wordSize; i++) {
        if (word1[i] != word2[i]) {
            count++;
        }
    }
    return count;
}

// @brief: finds the shortest word ladder between startWord and finalWord
// @param: word array, usedWord array (bool), size of word array, size of word, startWord, finalWord
// @return: shortest word ladder between startWord and finalWord
WordNode* findShortestWordLadder(   char** words,
                                    bool* usedWord,
                                    int numWords,
                                    int wordSize,
                                    char* startWord,
                                    char* finalWord ) {
    LadderNode* myList = NULL;
    WordNode* myLadder = NULL;

    insertWordAtFront(&myLadder, startWord);
    insertLadderAtBack(&myList, myLadder);

    while (myList != NULL) {
        myLadder = popLadderFromFront(&myList);
        for (int i = 0; i < numWords; i++) {
            if (countDifferences(words[i], myLadder->myWord, wordSize) == 1) {
                if (!usedWord[i]) {
                    usedWord[i] = true;
                    if (strcmp(words[i], finalWord) == 0) {
                        insertWordAtFront(&myLadder, finalWord);
                        freeLadderList(myList);
                        return myLadder;
                    }
                    else {
                        WordNode* anotherLadder = copyLadder(myLadder);
                        insertWordAtFront(&anotherLadder, words[i]);
                        insertLadderAtBack(&myList, anotherLadder);
                    }
                }
            }
        }
    }
    freeLadder(myLadder);
    freeLadderList(myList);
    return NULL;
}

// interactive user-input to set a word;
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize);
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}


//-----------------------------------------------------
// The primary application is fully provided in main();
//  no changes should be made to main()
//-----------------------------------------------------
int main() {
    srand((int)time(0));

    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n");
    printf("word ladder between two %d-letter words.\n\n",wordSize);

    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }

    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }

    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i]
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }

    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize);
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n");

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array,
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));

    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);

    return 0;
}
