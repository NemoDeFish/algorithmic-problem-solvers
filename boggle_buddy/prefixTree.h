struct prefixTree;

#ifndef PREFIXTREESTRUCT
#define PREFIXTREESTRUCT
#define CHILD_COUNT (1 << 8)
#define UNMARKED (0)
#define MARKED (1)

struct prefixTree;

struct prefixTree {
    char* data;
    struct prefixTree* children[256];
    int found;
};

#endif

struct prefixTree *newPrefixTree();

struct prefixTree *addWordToTree(struct prefixTree *pt, char *word);

void freePrefixTree(struct prefixTree* node);
