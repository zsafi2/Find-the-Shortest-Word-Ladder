/*-------------------------------------------
Program 5: Find the Shortest Word Ladder
Course: CS 211, Fall 2023, UIC
System: Visual Studio Code
Author: Zaheer Safi
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// struct for a linked list that stores a string myWord
typedef struct WordNode_struct 
{
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

// sturct for a linked list where each node is another linked list
typedef struct LadderNode_struct 
{
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

// the function opens a file reads all the words from it and return the number of words that has the size wordSize
int countWordsOfLength(char* filename, int wordSize) 
{ 
    // open the file given the filename
    FILE* input = fopen(filename, "r");

    // if the file did not open return -1
    if (input == NULL)
    {
        return -1;
    }

    // make temperary veriable word and then count to count the number of words
    char word[100];
    int count = 0;
    
    // read each words untill the end of the file
    while(fscanf(input, "%s", word) == 1)
    {
        // if the words has the same character length as wordSize the increment the count
        if (strlen(word) == wordSize)
        {
            count++;
        }
    }
    fclose(input);
    
    return count; 
}

// the function opens the file and reads all the words from the file that are the size wordSize and update the words array with it
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    
    // open the file
    FILE* input = fopen(filename, "r");

    // read it and count the number f words in it
    int size = countWordsOfLength(filename, wordSize);

    // if the file did not open return false
    if (input == NULL || size != numWords)
    {
        return false;
    }

    char word[100];
    int count = 0;
    
    // read each words and update the words array
    while(fscanf(input, "%s", word) == 1)
    {
        if (strlen(word) == wordSize)
        {
            strcpy(words[count], word);
            words[count][wordSize] = '\0'; // add a null character to the end of the word
            count++;
        }
    }
    fclose(input);
    
    return true;
}

// binary search on the words array to find the index of the aWord 
int findWord(char** words, char* aWord, int loInd, int hiInd) {
    
    while (loInd <= hiInd) 
    {
        // find the middle index of the array
        int mid = loInd + (hiInd - loInd) / 2;

        // check if the words is in the middle index
        int compareResult = strcmp(aWord, words[mid]);

        if (compareResult == 0) 
        {
            return mid; // Found the word
        } 
        
        else if (compareResult < 0) 
        {
            hiInd = mid - 1; // The word is in the left half
        } 
        
        else 
        {
            loInd = mid + 1; // The word is in the right half
        }
    }

    return -1; // Word not found between loInd and hiInd
}

// free the heap allocated words Array which has the size numWords
void freeWords(char** words, int numWords) 
{
    // go through each index and free the string stored there
    for(int i = 0; i < numWords; i++)
    {
        free(words[i]);
    }
    free(words); // free the words array itself
}

// function to insert a node to the frond of my Ladder linked list the data should be newWord
void insertWordAtFront(WordNode** ladder, char* newWord) 
{
    // allocate space for a new node
    WordNode* newNode = malloc(sizeof(WordNode));

    // intialize the data and the next pointer
    newNode->next = NULL;
    newNode->myWord = newWord;

    // if the linked lsit is empty
    if (*ladder == NULL)
    {
        *ladder = newNode; // make the newNode the first Node
        return;
    }

    // otherwise point the new Node to the first Node and the first Node to the newNode
    newNode->next = *ladder;
    *ladder = newNode;

}

// go through the linked list and count the number of nodes and return it
int getLadderHeight(WordNode* ladder) 
{
    // storng the first node in the curr
    WordNode* curr = ladder;
    int count = 0;

    // go through the linked list until the last node and increment the count veriable
    while (curr != NULL)
    {
        count++;
        curr = curr->next;
    }
    return count; 
}

// the parameter of the function is a linked list make a copy of the linked list and return the head or the first node
WordNode* copyLadder(WordNode* ladder) 
{
    // if the given linked list is empty then return null
    if (ladder == NULL)
    {
        return NULL;
    }
    
    // make a new linked list intialize with null
    WordNode* newLadder = NULL; 

    // store the first node of ladder in curr
    WordNode* curr = ladder;
   
    // traverse the linked list
    while (curr != NULL) 
    {
        // Create a new WordNode by allocating space
        WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
        newNode->myWord = curr->myWord;
        newNode->next = NULL;
        
        // handle the first node
        if (newLadder == NULL)
        {
            newLadder = newNode;
        }
        
        // otherwise append the node to the end of the new linked list
        else
        {
            WordNode* temp = newLadder;
            // traverse the new linked list
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newNode; // append the node
        }

        // Move to the next node in the original ladder
        curr = curr->next;
    }

    // Return the copied ladder
    return newLadder;
}

// given a linked list free all the nodes and the linked list itself
void freeLadder(WordNode* ladder) 
{
    // travese the linked list
    WordNode* curr = ladder;
    while (curr != NULL)
    {
        // store the node in a temperary veriable
        WordNode* temp = curr;
        // move to the next node
        curr = curr->next;
        // free the temprary veriable
        free(temp);
    }
}

// funciton that appends a new linked list node to the back of the LadderNode linked list
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) 
{
    // make a new Node (which is a linked list)
    LadderNode* newNode = (LadderNode*)malloc(sizeof(LadderNode));
    newNode->topWord = newLadder;
    newNode->next = NULL;
    
    // if our ladderNode linked list is empty make the new linked list the first node
    if (*list == NULL)
    {
        *list = newNode;
        return;
    }
    
    // otherwise traverse the linked list and append the node
    LadderNode* curr = *list;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = newNode;
}

// given a linked list remove the first node from the list and return the nodes data
WordNode* popLadderFromFront(LadderNode** list) 
{
    // if the linked lsit is empty so there is not item to remove
    if (*list == NULL)
    {
        return NULL;
    }
    
    // otherwise hold the first Node and its data in a temperary veriables
    LadderNode* Temp = *list;
    WordNode* Temp_data = (*list)->topWord;
    
    // change the head node point to the next one delete the pointer to the first node
    *list = (*list)->next;
    free(Temp);

    return Temp_data; // return the data
}

// give myLIst which is a linked lsit where every node is a linked list deallocate all the linked list memory
void freeLadderList(LadderNode* myList) 
{
    // traverse the linke list
    LadderNode* curr = myList;
    while (curr != NULL)
    {
        // remove every node which is a linked lsit and also remove it's data
        LadderNode* temp = curr;
        curr = curr->next;
        freeLadder(temp->topWord);
        free(temp);
        
    }
}

// given a start word and and ending word firnd the shortest path to get from the first word to the final word
// the rule is moveing from the start word to the end word the words in the middle should be different by one letter
// in this algorithm we are going to make a linked list were every node is a linked list which represent the paths we take
// and from there we find the shortes path and return the shrot path linked list
WordNode* findShortestWordLadder(char** words, bool* usedWord, int numWords, int wordSize, char* startWord, char* finalWord)
{
    // Create an empty linked list for ladders
    LadderNode* ladderList = NULL;
    WordNode* currentLadder = NULL;

    // Find the index of the startWord in the words array
    int startIndex = findWord(words, startWord, 0, numWords - 1);
    
    // Insert the startWord into the current ladder and add it to the ladder list
    insertWordAtFront(&currentLadder, words[startIndex]);
    insertLadderAtBack(&ladderList, currentLadder);
    usedWord[startIndex] = true;

    char tempWord[150];

    // Main loop to find the shortest ladder
    while (ladderList != NULL)
    {
        currentLadder = popLadderFromFront(&ladderList);
        // Iterate through each character position in the word
        for (int charIndex = 0; charIndex < wordSize; charIndex++)
        {
            // Try all possible character replacements (a to z)
            for (int charReplace = 0; charReplace < 26; charReplace++)
            {
                // Copy the current word to a temporary variable
                strcpy(tempWord, currentLadder->myWord);
                // Replace the character at the given position
                tempWord[charIndex] = 'a' + charReplace;

                // Find the index of the word in the words array
                int foundIndex = findWord(words, tempWord, 0, numWords - 1);

                if (foundIndex >= 0 && !usedWord[foundIndex])
                {
                    usedWord[foundIndex] = true;

                    if (strcmp(finalWord, words[foundIndex]) == 0)
                    {
                        // If the word matches the finalWord, add it to the ladder and return
                        insertWordAtFront(&currentLadder, words[foundIndex]);
                        freeLadderList(ladderList);
                        return currentLadder;
                    }
                    else
                    {
                        // Create a copy of the current ladder, add the new word, and add it to the ladder list
                        WordNode* newLadder = copyLadder(currentLadder);
                        insertWordAtFront(&newLadder, words[foundIndex]);
                        insertLadderAtBack(&ladderList, newLadder);
                    }
                }
            }
        }
        // Free the memory of the current ladder
        freeLadder(currentLadder);
    }

    // If no ladder is found, return NULL
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
