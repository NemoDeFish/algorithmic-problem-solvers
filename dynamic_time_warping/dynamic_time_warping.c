#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_time_warping.h"

#define LDINFINITY (LDBL_MAX / 2.0L)

/* Finds the minimum of three values. */
long double min(long double a, long double b, long double c);

struct solution *dynamicTimeWarping(struct problem *p) {
    struct solution *s = newSolution(p);
    // Initialise the lengths of sequences
    int n = p->seqALength;
    int m = p->seqBLength;

    // Create and initialise the DTW matrix with infinity
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < m + 1; j++) {
            s->matrix[i][j] = LDINFINITY;
        }
    }
    s->matrix[0][0] = 0;

    // Populate the DTW matrix
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            // Calculate cost using the absolute difference between
            // sequence points
            long double cost = fabsl(p->sequenceA[i - 1] - p->sequenceB[j - 1]);
            s->matrix[i][j] = cost + min(s->matrix[i - 1][j],       // Insertion
                                         s->matrix[i][j - 1],       // Deletion
                                         s->matrix[i - 1][j - 1]);  // Match
        }
    }

    // The DTW distance is in the bottom-right corner of the matrix
    s->optimalValue = s->matrix[n][m];

    return s;
}

/*
    Finds the minimum of three values.
*/
long double min(long double a, long double b, long double c) {
    long double min = a;  // Assume 'a' is the minimum initially

    // Compare 'b' and 'c' with the current minimum
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }

    return min;
}

struct solution *dynamicTimeWarpingWithWindow(struct problem *p) {
    struct solution *s = newSolution(p);
    // Initialise the lengths of sequences
    int n = p->seqALength;
    int m = p->seqBLength;

    // Create and initialise the DTW matrix with infinity
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < m + 1; j++) {
            s->matrix[i][j] = LDINFINITY;
        }
    }
    s->matrix[0][0] = 0;

    // Populate the DTW matrix
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (j <= i + p->windowSize && j >= i - p->windowSize) {
                // Calculate cost using the absolute difference between
                // sequence points
                long double cost =
                    fabsl(p->sequenceA[i - 1] - p->sequenceB[j - 1]);
                s->matrix[i][j] = cost + min(s->matrix[i - 1][j],  // Insertion
                                             s->matrix[i][j - 1],  // Deletion
                                             s->matrix[i - 1][j - 1]);  // Match
            }
        }
    }

    // The DTW distance is in the bottom-right corner of the matrix
    s->optimalValue = s->matrix[n][m];

    return s;
}

struct solution *dynamicTimeWarpingWithPathConstraint(struct problem *p) {
    struct solution *s = newSolution(p);
    // Initialise the lengths of sequences
    int n = p->seqALength;
    int m = p->seqBLength;

    // Create and initialise the DTW matrix with infinity
    long double ***dtwMatrix = (long double ***)malloc(
        (p->maximumPathLength + 1) * sizeof(long double **));
    for (int i = 0; i < p->maximumPathLength + 1; i++) {
        dtwMatrix[i] = (long double **)malloc((n + 1) * sizeof(long double *));
        for (int j = 0; j < n + 1; j++) {
            dtwMatrix[i][j] =
                (long double *)malloc((m + 1) * sizeof(long double));
        }
    }
    for (int i = 0; i < p->maximumPathLength + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            for (int k = 0; k < m + 1; k++) {
                dtwMatrix[i][j][k] = LDINFINITY;
            }
        }
    }
    for (int i = 0; i < p->maximumPathLength + 1; i++) {
        dtwMatrix[i][0][0] = 0;
    }

    long double minimumCost = LDINFINITY;
    // Populate the DTW matrix for each path length
    for (int k = 1; k <= p->maximumPathLength; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (i + j + 1 >= k) {
                    // Calculate cost using the absolute difference between
                    // sequence points
                    long double cost =
                        fabsl(p->sequenceA[i - 1] - p->sequenceB[j - 1]);
                    // Insertion
                    dtwMatrix[k][i][j] =
                        cost + min(dtwMatrix[k - 1][i - 1][j],
                                   // Deletion
                                   dtwMatrix[k - 1][i][j - 1],
                                   // Match
                                   dtwMatrix[k - 1][i - 1][j - 1]);
                }
            }
        }
        // At each iteration, check if the current minimum is the optimal
        // value of all path lengths
        if (dtwMatrix[k][n][m] <= minimumCost) {
            minimumCost = dtwMatrix[k][n][m];
        }
    }

    // Free the 3D DTW Matrix
    for (int i = 0; i < p->maximumPathLength + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            free(dtwMatrix[i][j]);
        }
        free(dtwMatrix[i]);
    }
    free(dtwMatrix);

    // The DTW distance is in the bottom-right corner of the matrix
    s->optimalValue = minimumCost;

    return s;
}