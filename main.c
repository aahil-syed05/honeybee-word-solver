/*-------------------------------------
Program: Generated spelling bee game and solver
Purpose: To build a spelling bee hive from user input or randomly
from a dictionary based fit word. Program can either optionally let
user play the game or solve it itself. 
Course: CS211, UIC
Author: Aahil Syed
-------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

const int MIN_HIVE_SIZE = 2;
const int MAX_HIVE_SIZE = 12;
const int MIN_WORD_LENGTH = 4;

typedef struct WordList_struct {
    char** words;
    int numWords;
    int capacity;
} WordList;

WordList* createWordList() {
    WordList* newList = malloc(sizeof(WordList));
    newList->capacity = 4;
    newList->numWords = 0;
    newList->words = malloc(newList->capacity * sizeof(char*));

    return newList;
}


//appendWord()
//Adds copy of newWord to the end of the wordlist
void appendWord(WordList* myList, char* newWord) {   
    if(myList->numWords == myList->capacity){
        int newCapacity = myList->capacity * 2;

        char** biggerArray = (char**)malloc(newCapacity * sizeof(char*));

        for(int i = 0; i < myList->numWords; i++){
            biggerArray[i] = myList->words[i];
        }

        free(myList->words);
        myList->words = biggerArray;
        myList->capacity = newCapacity;
    }

    myList->words[myList->numWords] = (char*)malloc((strlen(newWord)+ 1) * sizeof(char));
    strcpy(myList->words[myList->numWords], newWord);

    myList->numWords++;

}


//buildDictionary()
//opens the dictionary file, reads a word at a time, then adds words of length mid length into dictionaryList
int buildDictionary(char* filename, WordList* dictionaryList, int minLength) {
    FILE* filePtr = fopen(filename, "r");

    if(filePtr == NULL){
        return -1;
    }

    char tempWord[100];
    int longest = 0;

    while (fscanf(filePtr, "%s", tempWord) == 1) {
        if ((int)strlen(tempWord) >= minLength){
            appendWord(dictionaryList, tempWord);

            if((int)strlen(tempWord) > longest){
                longest = strlen(tempWord);
            }
        }
    }
    fclose(filePtr);

    if(dictionaryList->numWords == 0){
        return -1;
    }
    return longest;
}


//freeWordList()
//Free all heap memory owned by wordlist
void freeWordList(WordList* myList) {
    for (int i = 0; i < myList->numWords; i++){
        free(myList->words[i]);
    }

    free(myList->words);

    free(myList);
}
// findLetter()
// Searches a string for a character and returns the index
int findLetter(char str[], char let) {
    for(int i = 0; str[i] != '\0'; i++){
        if( str[i] == let){
            return i;
        }
    }
    return -1;
}


//buildHive()
//builds a hive string from another string by inserting letters in alphabetical order and skipping dups
void buildHive(char str[], char hive[]) {
    int hiveLen = 0;
    hive[0] = '\0';

    for(int i = 0; str[i] != '\0'; i++){
        char ch = str[i];

        if(findLetter(hive, ch) != -1){
            continue;
        }

        int pos = 0;
        while (pos < hiveLen && hive[pos] < ch){
            pos++;
        }

        for( int j = hiveLen; j >= pos; j--){
            hive[j + 1] = hive[j];
        }

        hive[pos] = ch;
        hiveLen++;
    }

}

//countUniqueLetters() 
//counts how many distinct characters appear in the string
int countUniqueLetters(char str[]) {
    int count = 0;
    char seen[100] = "";

    for( int i = 0; str[i] != '\0'; i++){
        if(findLetter(seen, str[i]) == -1){
            int len = strlen(seen);
            seen[len] = str[i];
            seen[len + 1] = '\0';
            count++;
        }
    }
    return count;

}


// findAllFitWords
//creates and returns a wordlist of all dictionary words that contain exactly hivesize unique letters
WordList* findAllFitWords(WordList* dictionaryList, int hiveSize) {
    WordList* fitWords = createWordList();
    for (int i = 0; i < dictionaryList->numWords; i++){
        if(countUniqueLetters(dictionaryList->words[i]) == hiveSize){
            appendWord(fitWords, dictionaryList->words[i]);
        }
    }
    return fitWords;
}

// isValidWord
// returns true only if the word uses letters from hive and includes required letter
bool isValidWord(char word[], char hive[], char reqLet) {
    bool hasReq = false;

    if((int)strlen(word) < MIN_WORD_LENGTH){
        return false;
    }
    for (int i = 0; word[i] != '\0'; i++){
        if(findLetter(hive, word[i]) == -1){
            return false;
        }
        if(word[i] == reqLet){
            hasReq = true;
        }
    }
    return hasReq;
}

//isPangram
// returns true if the word uses every hive letter atleast once
bool isPangram(char word[], char hive[]) {
    for (int i = 0; hive[i] != '\0'; i++){
        if(findLetter(word, hive[i]) == -1){
            return false;
        }
    }

    return true;
}


void printHive(char hive[], int reqLetInd, char reqLet) {
    printf("  Hive: \"%s\"\n", hive);

    int baseLen = strlen("  Hive: \"");
    for(int i = 0; i < baseLen + reqLetInd; i++){
    printf(" ");
    }
    printf("^        (all words must include '%c')\n", reqLet);  
}


void printReqPointerForHiveLine( char hive[], int reqLetInd){
    printf("  Valid words from hive \"%s\":\n", hive);

        int baseLen = strlen("  Valid words from hive \"");
        for (int i = 0; i < baseLen + reqLetInd; i++) {
            printf(" ");
        }
        printf("^\n");
}

void printReqPointerForCountLine(char hive[], int reqLetInd){
        printf("  Total counts for hive \"%s\":\n", hive);

        int baseLen = strlen("  Total counts for hive \"");
        for (int i = 0; i < baseLen + reqLetInd; i++){
            printf(" ");
        }
        printf("^\n");
}

int getScore( char word[], char hive[]){
    int len = strlen(word);
    int score;
    
    if (len == MIN_WORD_LENGTH){
        score = 1;
    }
    else{
        score = len;
    }
    if(isPangram(word, hive)){
        score += strlen(hive);
    }
    return score;
}

bool isPerfectPangram(char word[], char hive[]){
    return isPangram(word, hive) && ((int)strlen(word) == (int)strlen(hive));
}

//printlist
//Prints all words in list with score formatting
void printList(WordList* thisWordList, char* hive) {
    printf("  Word List:\n");
    int totScore = 0;

    for( int i = 0; i < thisWordList->numWords; i++){
        char* word = thisWordList->words[i];
        int score = getScore(word, hive);

        if (isPerfectPangram(word, hive)){
            printf("  *** (%2d) %s\n", score, word);

        }
        else if(isPangram(word, hive)){
            printf("   *  (%2d) %s\n", score, word);
        }
        else{
            printf("      (%2d) %s\n", score, word);
        }
        totScore += score;
    }
    printf("  Total Score: %d\n", totScore);

}

void bruteForceSolve(WordList* dictionaryList, WordList* solvedList, char* hive, char reqLet) {
    for( int i = 0; i < dictionaryList->numWords; i++){
        if (isValidWord(dictionaryList->words[i], hive, reqLet)){
            appendWord(solvedList, dictionaryList->words[i]);
        }
    }
}

bool isPrefix(char* partWord, char* fullWord) {
    int i = 0;
    while( partWord[i] != '\0'){
        if(fullWord[i] == '\0' || partWord[i] != fullWord[i]){
            return false;
        }
        i++;
    }
    return true;
}

int findWord(WordList* thisWordList, char* aWord, int loInd, int hiInd) {
    if (hiInd < loInd) {
        if (loInd < thisWordList->numWords && isPrefix(aWord, thisWordList->words[loInd])) {
            return -1;
        }
        return -99; // (no match)
    }

    int mdInd = (loInd + hiInd) / 2;
    int cmp = strcmp(aWord, thisWordList->words[mdInd]);

    if (cmp == 0) { // Base case 1: found tryWord at midInd
        return mdInd;
    }
    if( cmp < 0){
        int leftResult = findWord(thisWordList, aWord, loInd, mdInd - 1);

        if(leftResult != -99){
            return leftResult;
        }
        if(isPrefix(aWord, thisWordList->words[mdInd])){
            return -1;
        }
        return -99;
    }
    return findWord(thisWordList, aWord, mdInd + 1, hiInd);
}

void findAllMatches(WordList* dictionaryList, WordList* solvedList, char* tryWord, char* hive, char reqLet) {
    int status = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1);

    if (status >= 0) {
        if (isValidWord(tryWord, hive, reqLet)){
            appendWord(solvedList, tryWord);
        }
        int len = strlen(tryWord);
        tryWord[len] = hive[0];
        tryWord[len + 1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        return;
    }

    if (status == -1){
        int len = strlen(tryWord);
        tryWord[len] = hive[0];
        tryWord[len + 1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        return;
    }

    int len = strlen(tryWord);

    while(len > 0){
        int pos = findLetter(hive, tryWord[len - 1]);

        if(pos < (int)strlen(hive) - 1){
            tryWord[len - 1] = hive[pos + 1];
            findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
            return;
        }

        tryWord[len - 1] = '\0';
        len--;
    }
}

   
 


// Provided for you, to determine the program settings based on parameters
bool setSettings(int argc, char* argv[], bool* pRandMode, int* pNumLets, char dictFile[100], bool* pPlayMode, bool* pBruteForceMode, bool* pSeedSelection) {
    *pRandMode = false;
    *pNumLets = 0;
    strcpy(dictFile, "dictionary.txt");
    *pPlayMode = false;
    *pBruteForceMode = true;
    *pSeedSelection = false;
    srand((int)time(0));
    //--------------------------------------
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pRandMode = true;
            *pNumLets = atoi(argv[i]);
            if (*pNumLets < MIN_HIVE_SIZE || *pNumLets > MAX_HIVE_SIZE) {
                return false;
            }
        }
        else if (strcmp(argv[i], "-d") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            strcpy(dictFile, argv[i]);
            FILE* filePtr = fopen(dictFile, "r");
            if (filePtr == NULL) {
                return false;
            }
            fclose(filePtr);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pSeedSelection = true;
            int seed = atoi(argv[i]);
            srand(seed);
        }
        else if (strcmp(argv[i], "-p") == 0) {
            *pPlayMode = true;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            *pBruteForceMode = false;
        }
        else {
            return false;
        }
    }
    return true;
}

void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}

void printYESorNO(bool mode) {
    if (mode) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
}



int main(int argc, char* argv[]) {

    printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");

    bool randMode = false;
    int hiveSize = 0;
    char dict[100] = "dictionary.txt";
    bool playMode = false;
    bool bruteForce = true;
    bool seedSelection = false;
    char hive[MAX_HIVE_SIZE + 1] = {};
    hive[0] = '\0';
    int reqLetInd = -1;
    char reqLet = '\0';


    if (!setSettings(argc, argv, &randMode, &hiveSize, dict, &playMode, &bruteForce, &seedSelection)) {
        printf("Invalid command-line argument(s).\nTerminating program...\n");
        return 1;
    }
    else {
        printf("Program Settings:\n");
        printf("  random mode = ");
        printONorOFF(randMode);
        printf("  play mode = ");
        printONorOFF(playMode);
        printf("  brute force solution = ");
        printONorOFF(bruteForce);
        printf("  dictionary file = %s\n", dict);
        printf("  hive set = ");
        printYESorNO(randMode);
        printf("\n\n");
    }

    // build word array (only words with desired minimum length or longer) from dictionary file
    printf("Building array of words from dictionary... \n");
    WordList* dictionaryList = createWordList();
    int maxWordLength = buildDictionary(dict, dictionaryList, MIN_WORD_LENGTH);
    if (maxWordLength == -1) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of valid words.\n");
        printf("Terminating program...\n");
        return 0;
    }
    printf("   Word array built!\n\n");


    printf("Analyzing dictionary...\n");

    if (dictionaryList->numWords < 0) {
        printf("  Dictionary %s not found...\n", dict);
        printf("Terminating program...\n");
        return 0;
    }

    // end program if file has zero words of minimum desired length or longer
    if (dictionaryList->numWords == 0) {
        printf("  Dictionary %s contains insufficient words of length %d or more...\n", dict, MIN_WORD_LENGTH);
        printf("Terminating program...\n");
        return 0;
    }
    else {
        printf("  Dictionary %s contains \n  %d words of length %d or more;\n", dict, dictionaryList->numWords, MIN_WORD_LENGTH);
    }




    if (randMode) {
        printf("==== SET HIVE: RANDOM MODE ====\n");
        //find number of words in words array that use hiveSize unique letters
        WordList* fitWords = findAllFitWords(dictionaryList, hiveSize);
        int numFitWords = fitWords->numWords;
        //pick one at random
        int pickOne = rand() % numFitWords;
        char* chosenFitWord = fitWords->words[pickOne];

        //and alaphabetize the unique letters to make the letter hive
        buildHive(chosenFitWord, hive);
        freeWordList(fitWords);

        reqLetInd = rand() % hiveSize;
        reqLet = hive[reqLetInd];

    }
    else {
        printf("==== SET HIVE: USER MODE ====\n");
        char inputLets[100];
        bool validHive = false;

        while(!validHive){
            printf("  Enter a single string of lower-case,\n");
            printf("  unique letters for the letter hive... ");
            scanf("%s", inputLets);

            validHive = true;
            int len = strlen(inputLets);

            if(len < MIN_HIVE_SIZE || len > MAX_HIVE_SIZE){
                printf("  HIVE ERROR: \"%s\" has invalid length;\n", inputLets);
                printf("  valid hive size is between %d and %d, inclusive\n", MIN_HIVE_SIZE, MAX_HIVE_SIZE);
                validHive = false;
            }
            for ( int i = 0; i < len && validHive; i++) {
                if(!islower(inputLets[i])){
                    printf("  HIVE ERROR: \"%s\" contains invalid letters;\n", inputLets);
                    printf("  valid characters are lower-case alpha only\n");
                    validHive = false;
                }
            }

            for(int i = 0; i < len && validHive; i++){
                for (int j = i + 1; j < len && validHive; j++){
                    if(inputLets[i] == inputLets[j]){
                        printf("  HIVE ERROR: \"%s\" contains duplicate letters\n", inputLets);
                        validHive = false;
                    }
                }
            }
        }
        printf("\n");
        buildHive(inputLets, hive);

        bool validReq = false;
        char tempReq;

        while(!validReq){
            printf("  Enter the letter from \"%s\"\n", hive);
            printf("  that is required for all words: ");
            scanf(" %c", &tempReq);

            tempReq = tolower(tempReq);

            if(findLetter(hive, tempReq) == -1){
                printf("  HIVE ERROR: \"%s\" does not contain the character '%c'\n", hive, tempReq);
            }
            else{
                reqLet = tempReq;
                reqLetInd = findLetter(hive, reqLet);
                validReq = true;
            }
        }
        printHive(hive, reqLetInd, reqLet);
    }

    if (playMode) {
        printf("==== PLAY MODE ====\n");

        char* userWord = (char*)malloc((maxWordLength + 1) * sizeof(char));
        strcpy(userWord, "default");

        WordList* userWordList = createWordList();

        printf("............................................\n");
        printHive(hive, reqLetInd, reqLet);
        printf("  Enter a word (enter DONE to quit): ");
        scanf("%s", userWord);
        printf("\n");


        while (strcmp(userWord, "DONE") != 0) {
            for(int i = 0; userWord[i] != '\0'; i++){
                userWord[i] = tolower(userWord[i]);
            }
            if((int)strlen(userWord) < MIN_WORD_LENGTH){
            printf(" !!! \"%s\" is TOO SHORT (min length = %d) !!!\n", userWord, MIN_WORD_LENGTH);
            }
            else{
                bool inDictionary = false;
                for( int i = 0; i < dictionaryList->numWords; i++){
                    if(strcmp(userWord, dictionaryList->words[i]) == 0){
                        inDictionary = true;
                        break;
                    }
                }

                if (!inDictionary){
                    printf(" !!! \"%s\" is NOT a valid word in the dictionary !!!\n", userWord);
                }
                else{
                    bool alreadyUsed = false; 
                    for (int i = 0; i < userWordList->numWords; i++){
                        if(strcmp(userWord, userWordList->words[i]) == 0){
                            alreadyUsed = true;
                            break;
                        }
                    }

                    if(alreadyUsed){
                        printf(" !!! \"%s\" is already in your word list !!!\n", userWord);
                    }
                    else if (!isValidWord(userWord, hive, reqLet)){
                        printf(" !!! \"%s\" is NOT valid for the hive !!!\n", userWord);
                    }
                    else{
                        appendWord(userWordList, userWord);
                    }
                }
            }
            // Keep the code below this comment, which prints the list and the hive, and gets the next input
            printf("\n");
            printList(userWordList, hive);
            printf("............................................\n");
            printHive(hive, reqLetInd, reqLet);

            printf("  Enter a word (enter DONE to quit): ");
            scanf("%s", userWord);
            printf("\n");

        }

        // Do until user enters quit request
        //  Display Hive & Word List & Current Score
        //  X Allow user to enter a word
        //  X Check if word is in dict
        //  X Check if word is a duplicate
        //  Check if word is valid for the hive
        //  If all valid, add it to word list, report score  

        freeWordList(userWordList);
        free(userWord);
    }

    // Helpful variables
    int numValidWords = 0;
    int numPangrams = 0;
    int numPerfectPangrams = 0;
    int totScore = 0;
    //int score = 0;
    bool isBingo = true;

    WordList* solvedList = createWordList();

    if (bruteForce) { //find all words that work... (1) brute force
        bruteForceSolve(dictionaryList, solvedList, hive, reqLet);
    }
    else {
        char* tryWord = (char*)malloc(sizeof(char) * (maxWordLength + 1));
        tryWord[0] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        free(tryWord);
    }



    int longestSolvedWordLen = 0;
    for (int i = 0; i < solvedList->numWords; i++) {
        if ((int)strlen(solvedList->words[i]) > longestSolvedWordLen) {
            longestSolvedWordLen = strlen(solvedList->words[i]);
        }
    }

    numValidWords = solvedList->numWords;
    isBingo = true;

    bool startsFound[26] = {false};

    for (int i = 0; i < solvedList->numWords; i++){
        char* word = solvedList->words[i];

        if(isPangram(word, hive)){
            numPangrams++;
        }
        if(isPerfectPangram(word, hive)){
            numPerfectPangrams++;
        }
        totScore += getScore(word, hive);
        startsFound[word[0] - 'a'] = true;
    }

    for (int i = 0; hive[i] != '\0'; i++){
        if(!startsFound[hive[i] - 'a']){
            isBingo = false;
        }
    }
    
    printf("==== SPELLING BEE SOLVER ====\n");
    printReqPointerForHiveLine(hive, reqLetInd);
    printList(solvedList, hive);
    printReqPointerForCountLine(hive, reqLetInd);
    printf("    Number of Valid Words: %d\n", numValidWords);
    printf("    Number of ( * ) Pangrams: %d\n", numPangrams);
    printf("    Number of (***) Perfect Pangrams: %d\n", numPerfectPangrams);
    printf("    Bingo: ");
    printYESorNO(isBingo);
    printf("    Total Score Possible: %d\n", totScore);

    freeWordList(dictionaryList);
    freeWordList(solvedList);
    return 0;
}

