#include "prefixTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

struct prefixTree *newPrefixTree(){
    /* Structure of the prefix tree includes a dummy node. */
    struct prefixTree *retTree = NULL;
    retTree = (struct prefixTree *)malloc(sizeof(struct prefixTree));
    for (int i = 0; i < 256; i++) {
        retTree->children[i] = NULL;
    }
    retTree->data = malloc(1 * sizeof(char));
    retTree->data[0] = '\0';
    retTree->found = UNMARKED;
    return retTree;
}

struct prefixTree *addWordToTree(struct prefixTree *pt, char *word){
    struct prefixTree *temp = pt;

    for (int i = 0; i < strlen(word) + 1; i++) {
        int idx = (int)word[i];
        if (temp->children[idx] == NULL) {
            temp->children[idx] = newPrefixTree();
            int len = strlen(temp->data);
            temp->children[idx]->data = realloc(temp->children[idx]->data, (len + 2) * sizeof(char));
            strcpy(temp->children[idx]->data, temp->data);
            temp->children[idx]->data[len] = word[i];
            temp->children[idx]->data[len + 1] = '\0';
        }
        temp = temp->children[idx];
    }
    return pt;
}

void freePrefixTree(struct prefixTree* node) {
    // Free the trienode sequence
    for(int i = 0; i < 256; i++) {
        if (node->children[i] != NULL) {
            freePrefixTree(node->children[i]);
        }
        else {
            continue;
        }
    }
    free(node);
}

