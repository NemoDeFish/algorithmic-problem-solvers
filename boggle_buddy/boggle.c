#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prefixTree.h"
#include "boggle.h"

/* Checks that the index are within the dimensions and are not viisted. */
int isSafe(char **board, int row, int col, int dimension, int **visited);

/* Traverses each cell of the board for finding all valid words. */
void findAllWordsInBoard(char **board, int dimension, struct prefixTree *tree);

/* Implements DFS for finding all valid words. */
void findAllWordsDFS(char **board, int row, int col, int dimension, int **visited,
              struct prefixTree *tree);

/* Traverses each cell of the board for autocomplete partial words. */
void autocompleteTraversal(char **board, int dimension, struct prefixTree *tree,
                        char *partialString);

/* Implements DFS for autocomplete partial words. */
void autocompleteDFS(char **board, int row, int col, int dimension, int **visited,
              struct prefixTree *tree, char *partialString,
              int indexPartialString);

/* Traverses each cell of the board for longest valid word. */
void findLongestWordTraversal(char **board, int dimension, struct prefixTree *tree);

/* Finds all next possible characters from a given prefix tree node. */
void getNextPossibleLetters(struct prefixTree *node, struct solution *s);

/* Finds the prefix tree node corresponding to the prefix last character. */
struct prefixTree *findNode(struct prefixTree *root, char *prefix);

/* Implements DFS for longest valid word. */
void findLongestWordDFS(char **board, int dimension, int **visited,
              struct prefixTree *tree, int *builtWord, int **current);

/* Traverses the prefix trie to find all marked words. */
void collectMarkedWords(struct prefixTree *node, char *prefix, int level,
               struct solution *s);

/* Dynamically allocate memory for a 2D matrix. */
int **createMatrix(int rows, int cols);

/* Reset a 2D matrix to 0. */
void resetMatrix(int **matrix, int rows, int cols);

/* Display a 2D matrix. */
void displayMatrix(int **matrix, int rows, int cols);

/* Mark next letters that are next to a 1 in the integer matrix. */
int **markAdjacentOnes(char **board, int dimension, int **visited,
                       int **current, char lowercaseLetter);

struct solution *findAllValidWords(struct problem *p) {
    struct solution *s = newSolution(p);
    struct prefixTree *tree = newPrefixTree();
    // Add words to the prefix trie
    for (int i = 0; i < p->wordCount; i++) {
        tree = addWordToTree(tree, p->words[i]);
    }

    findAllWordsInBoard(p->board, p->dimension, tree);
    char prefix[100];
    collectMarkedWords(tree, prefix, 0, s);
    return s;
}

/*
    Implements DFS for finding all valid words.
*/
void findAllWordsDFS(char **board, int row, int col, int dimension, int **visited,
              struct prefixTree *tree) {
    // Mark this cell as visited
    visited[row][col] = 1;
    // Check each time if the word is terminated, if it is, mark it
    if (tree->children[0] && tree->children[0]->found == UNMARKED) {
        tree->children[0]->found = MARKED;
    }

    // Arrays used to get row and column of 8 neighbors for a given cell
    int rowNum[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int colNum[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Recur for all connected neighbors
    for (int k = 0; k < 8; k++) {
        int newRow = row + rowNum[k];
        int newCol = col + colNum[k];
        // Check whether it is in the boundaries and it is not visited
        if (isSafe(board, newRow, newCol, dimension, visited)) {
            int lowercaseLetter = board[newRow][newCol] + 32;
            // Check if that letter is a subsequent letter in the prefix trie
            if (tree->children[lowercaseLetter]) {
                autocompleteTraversal(board, newRow, newCol, dimension, visited,
                         tree->children[lowercaseLetter]);
                // Mark the backtracked cell as unvisited for future visits
                visited[newRow][newCol] = 0;
            }
        }
    }
}

/*
    Traverses each cell of the board for finding all valid words.
*/
void findAllWordsInBoard(char **board, int dimension, struct prefixTree *tree) {
    // Matrix to mark visited cells. Initially all cells are unvisited
    int **visited = createMatrix(dimension, dimension);

    // Traverse through all cells of given matrix
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            // Reset the visited cells at each traversal
            resetMatrix(visited, dimension, dimension);

            int lowercaseLetter = board[i][j] + 32;
            // Check if that letter is a subsequent letter in the prefix trie
            if (tree->children[lowercaseLetter]) {
                // Visit all cells in this connected component
                autocompleteTraversal(board, i, j, dimension, visited,
                         tree->children[lowercaseLetter]);
            }
        }
    }
}

struct solution *autocompletePartialWord(struct problem *p) {
    struct solution *s = newSolution(p);
    struct prefixTree *tree = newPrefixTree();
    // Add words to the prefix trie
    for (int i = 0; i < p->wordCount; i++) {
        tree = addWordToTree(tree, p->words[i]);
    }

    autocompleteTraversal(p->board, p->dimension, tree, p->partialString);
    // Find the node corresponding to the last character of the prefix
    struct prefixTree *prefixNode = findNode(tree, p->partialString);
    if (prefixNode == NULL) {
        printf("Prefix not found in the Trie.\n");
    } else {
        // Find the next possible characters
        getNextPossibleLetters(prefixNode, s);
    }
    return s;
}

/*
    Implements DFS for autocomplete partial words.
*/
void autocompleteDFS(char **board, int row, int col, int dimension, int **visited,
              struct prefixTree *tree, char *partialString,
              int indexPartialString) {
    // Base case: we traverse until we reach the end of the partial string
    if (indexPartialString == strlen(partialString)) {
        // Mark this cell as visited
        visited[row][col] = 1;
        // Check each time if the word is terminated, if it is, mark it
        if (tree->children[0] && tree->children[0]->found == UNMARKED) {
            tree->children[0]->found = MARKED;
            tree->found = MARKED;
        }
    } else {
        // Else, we continue to traverse the partial string
        indexPartialString++;
    }

    // Mark this cell as visited
    visited[row][col] = 1;

    // Arrays used to get row and column of 8 neighbors for a given cell
    int rowNum[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int colNum[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Recur for all connected neighbors
    for (int k = 0; k < 8; k++) {
        int newRow = row + rowNum[k];
        int newCol = col + colNum[k];
        // Check whether it is in the boundaries and it is not visited
        if (isSafe(board, newRow, newCol, dimension, visited)) {
            int lowercaseLetter = board[newRow][newCol] + 32;
            // Check if that letter is a subsequent letter in the prefix trie
            if (tree->children[lowercaseLetter]) {
                if (indexPartialString == strlen(partialString) ||
                    lowercaseLetter == partialString[indexPartialString - 1]) {
                    autocompleteDFS(board, newRow, newCol, dimension, visited,
                             tree->children[lowercaseLetter], partialString,
                             indexPartialString);

                    // Mark the backtracked cell as unvisited for future visits
                    visited[newRow][newCol] = 0;
                    // Mark word if terminated
                    if (tree->children[lowercaseLetter]->found == MARKED) {
                        tree->found = MARKED;
                    }
                }
            }
        }
    }
}

/*
    Traverses each cell of the board for autocomplete partial words.
*/
void autocompleteTraversal(char **board, int dimension, struct prefixTree *tree,
                        char *partialString) {
    // Matrix to mark visited cells. Initially all cells are unvisited
    int **visited = createMatrix(dimension, dimension);

    // Traverse through the all cells of given matrix
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            // Reset the visited cells
            resetMatrix(visited, dimension, dimension);

            int lowercaseLetter = board[i][j] + 32;
            // Check if that letter is a subsequent letter in the prefix trie
            if (board[i][j] && !visited[i][j] &&
                tree->children[lowercaseLetter] &&
                lowercaseLetter == partialString[0]) {
                // Visit all cells in this connected component
                autocompleteDFS(board, i, j, dimension, visited,
                         tree->children[lowercaseLetter], partialString, 1);
            }
        }
    }
}

/*
    Finds the prefix tree node corresponding to the prefix last character.
*/
struct prefixTree *findNode(struct prefixTree *root, char *prefix) {
    struct prefixTree *current = root;

    // Loop until we reach the end of the prefix
    while (*prefix) {
        int index = *prefix;
        if (current->children[index] == NULL) {
            return NULL;  // Prefix not found
        }
        current = current->children[index];
        prefix++;  // Next character on prefix
    }
    return current;
}

/*
    Finds all next possible characters from a given prefix tree node.
*/
void getNextPossibleLetters(struct prefixTree *node, struct solution *s) {
    for (int i = 0; i < 256; i++) {
        // Check if there's a children first and if that children is marked
        if (node->children[i] && node->children[i]->found == MARKED) {
            s->followLetters = realloc(
                s->followLetters, (s->foundLetterCount + 1) * sizeof(char));
            s->followLetters[s->foundLetterCount] = (char)i;
            s->foundLetterCount++;
        }
    }
}

struct solution *findLongestValidWord(struct problem *p) {
    struct solution *s = newSolution(p);
    struct prefixTree *tree = newPrefixTree();
    for (int i = 0; i < p->wordCount; i++) {
        tree = addWordToTree(tree, p->words[i]);
    }

    findLongestWordTraversal(p->board, p->dimension, tree);
    char prefix[100];
    collectMarkedWords(tree, prefix, 0, s);
    return s;
}

/*
    Traverses the prefix trie to find all marked words.
*/
void collectMarkedWords(struct prefixTree *node, char *prefix, int level,
               struct solution *s) {
    // If a node in the prefix trie is marked found, add it into s->words
    if (node->found) {
        s->words = realloc(s->words, (s->foundWordCount + 1) * sizeof(char *));
        s->words[s->foundWordCount] = (char *)malloc(strlen(node->data) + 1);
        strcpy(s->words[s->foundWordCount], node->data);
        s->foundWordCount++;
    }

    // Use DFS to find all the words recursively
    for (int i = 0; i < 256; i++) {
        if (node->children[i]) {
            prefix[level] = 'a' + i;
            collectMarkedWords(node->children[i], prefix, level + 1, s);
        }
    }
}

/*
    Checks that the index are within the dimensions and are not viisted.
*/
int isSafe(char **board, int row, int col, int dimension, int **visited) {
    return (row >= 0) && (row < dimension) && (col >= 0) && (col < dimension) &&
           (board[row][col] && !visited[row][col]);
}

/*
    Implements DFS for finding logest valid word.
*/
void findLongestWordDFS(char **board, int dimension, int **visited,
              struct prefixTree *tree, int *builtWord, int **current) {
    // Check each time if the word is terminated, if it is, mark it
    if (tree->children[0] && tree->children[0]->found == UNMARKED) {
        tree->children[0]->found = MARKED;
    }

    // Arrays used to get row and column of 8 neighbors for a given cell
    int rowNum[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int colNum[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Create another matrix to store the subsequent letters
    int **next = createMatrix(dimension, dimension);
    resetMatrix(next, dimension, dimension);

    // Create another matrix to store considered letters to avoid repetition
    int consideredLetters[256] = {0};

    // Traverse current matrix and find cells equal to 1
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (current[i][j] == 1) {
                // Mark this cell as visited
                visited[i][j] = 1;

                // Recur for all connected neighbors
                for (int k = 0; k < 8; k++) {
                    int newRow = i + rowNum[k];
                    int newCol = j + colNum[k];
                    // Check whether it is in the boundaries and not visited
                    if (isSafe(board, newRow, newCol, dimension, visited)) {
                        int lowercaseLetter = board[newRow][newCol] + 32;
                        // Check if that letter is a subsequent letter in the
                        // prefix trie
                        if (tree->children[lowercaseLetter] &&
                            !builtWord[lowercaseLetter] &&
                            !consideredLetters[lowercaseLetter]) {
                            // Mark the letter to ensure uniqueness
                            builtWord[lowercaseLetter] = 1;
                            // Mark the letter to ensure we don't consider
                            // it again in the future
                            consideredLetters[lowercaseLetter] = 1;
                            // Find all same letters that on the board
                            next = markAdjacentOnes(board, dimension, visited,
                                                    current, lowercaseLetter);
                            findLongestWordDFS(board, dimension, visited,
                                     tree->children[lowercaseLetter], builtWord,
                                     next);
                            // Mark the backtracked cell as not repeated for
                            // future visits
                            builtWord[lowercaseLetter] = 0;
                        }
                    }
                }
                // Mark the backtracked cell as unvisited for future visits
                visited[i][j] = 0;
            }
        }
    }
}

// Mark next letters that are next to a 1 in the integer matrix
int **markAdjacentOnes(char **board, int dimension, int **visited,
                       int **current, char lowercaseLetter) {
    // Arrays used to get row and column of 8 neighbors for a given cell
    int rowNum[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int colNum[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Create another matrix to store the subsequent letters
    int **next = createMatrix(dimension, dimension);
    resetMatrix(next, dimension, dimension);

    // Traverse current matrix and find cells equal to 1
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (current[i][j] == 1) {
                // Recur for all connected neighbors
                for (int k = 0; k < 8; k++) {
                    int newRow = i + rowNum[k];
                    int newCol = j + colNum[k];
                    // Check whether it is in the boundaries and it is not
                    // visited
                    if (isSafe(board, newRow, newCol, dimension, visited)) {
                        // Check if that letter is a subsequent letter in the
                        // prefix trie
                        if ((32 + board[newRow][newCol]) == lowercaseLetter) {
                            // Find all same letters that on the board
                            next[newRow][newCol] = 1;
                        }
                    }
                }
            }
        }
    }
    return next;
}

/*
    Traverses each cell of the board for finding longest valid word.
*/
void findLongestWordTraversal(char **board, int dimension, struct prefixTree *tree) {
    // Matrix to mark visited cells. Initially all cells are unvisited
    int **visited = createMatrix(dimension, dimension);
    int **current = createMatrix(dimension, dimension);

    // Traverse through all cells of given matrix
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            // Reset the visited and current cells at each traversal
            resetMatrix(visited, dimension, dimension);
            resetMatrix(current, dimension, dimension);

            // Create a boolean array to store presence of unique characters
            int builtWord[256] = {0};

            int lowercaseLetter = board[i][j] + 32;
            // Check if that letter is a subsequent letter in the prefix trie
            if (board[i][j] && !visited[i][j] &&
                tree->children[lowercaseLetter]) {
                // Mark the letter as the current letter for the first
                // iteration of DFS and mark the unique letter to ensure
                // uniqueness
                builtWord[lowercaseLetter] = 1;
                current[i][j] = 1;
                // Visit all cells in this connected component
                findLongestWordDFS(board, dimension, visited,
                         tree->children[lowercaseLetter], builtWord, current);
            }
        }
    }
}

/*
    Dynamically allocate memory for a 2D matrix
*/
int **createMatrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }
    return matrix;
}

/*
    Display a 2D matrix.
*/
void displayMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
    Reset a 2D matrix to 0.
*/
void resetMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 0;
        }
    }
}
