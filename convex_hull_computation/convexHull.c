/*  convexHull.c
    Implemented by Lim Si Yong

    Implementation details for module which contains convex hull specification
    data structures and functions.
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "convexHull.h"
#include "linkedList.h"
#include "problem.h"

/* Finds the index of the lowest point in the points1 array. If there are
   multiple lowest points, choose the one with the lowest point in the points2
   array. */
int findLowest(long double *points1, long double *points2, int numPoints);

/* Calculates the polar angle of each point with respect to the lowest point. */
long double calcPolar(long double jX, long double jY, long double minX,
                      long double minY);

/* Swap two points. */
void swap(long double *a, long double *b);

/* Gets the topmost point in the stack while retaining the stack's status. */
void *top(struct stack **s);

/* Gets the second topmost point in the stack while retaining the stack's
 * status. */
void *secondTop(struct stack **s);

/* Merges two subarrays into a single array. */
void merge(struct problem* p, int* arr, int left, int middle, int right,
           struct solution* s);

/* Algorithm for merge sort to sort an array. */
void mergeSort(struct problem* p, int* arr, int left, int right,
               struct solution* s);

/* Perform Jarvis' March to construct a convex hull for the given problem. */
struct solution *jarvisMarch(struct problem *p) {
    struct linkedList *hull = NULL;
    struct solution *s = (struct solution *)malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;

    /* Ensure there are at least 3 points. */
    if (p->numPoints < 3) {
        s->convexHull = hull;
        return s;
    }

    /* Initialize an empty list to store convex hull points. */
    hull = newList();

    /* Find the leftmost point (pivot) among the given points. */
    int leftmost = 0;
    leftmost = findLowest(p->pointsX, p->pointsY, p->numPoints);

    /* Start from the leftmost point. */
    int current = 0, nextPoint = 0;
    long double currentX = p->pointsX[leftmost];
    long double currentY = p->pointsY[leftmost];
    current = leftmost;

    do {
        /* Add current point to the convex hull. */
        insertTail(hull, currentX, currentY);

        /* Find the next point 'nextPoint' such that it forms a counterclockwise
         * turn with the current point and any other point in the set. */
        nextPoint = 0;

        for (int point = 0; point < p->numPoints; point++) {
            s->operationCount++;
            if (nextPoint == current ||
                orientation(p, current, nextPoint, point) == 2) {
                nextPoint = point;
            }
        }

        /* Set 'nextPoint' as the current point for the next iteration. */
        current = nextPoint;

        currentX = p->pointsX[nextPoint];
        currentY = p->pointsY[nextPoint];

        /* Repeat until we return to the starting point (leftmost). */
    } while (current != leftmost);

    // printf("%d", s->operationCount);

    /* Return the list of points in the convex hull. */
    s->convexHull = hull;
    return s;
}

/* Perform Graham's Scan to construct a convex hull for the given problem. */
struct solution *grahamScan(struct problem *p) {

    struct linkedList *hull = NULL;
    struct solution *s = (struct solution *)malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;

    /* Ensure there are at least 3 points. */
    if (p->numPoints < 3) {
        s->convexHull = hull;
        return s;
    }

    /* Find the point with the lowest y-coordinate. */
    int lowest = 0;
    lowest = findLowest(p->pointsY, p->pointsX, p->numPoints);

    /* Since we already know the index of the lowest element, we switch that to
     * the start of the array. */
    swap(&(p->pointsX[0]), &(p->pointsX[lowest]));
    swap(&(p->pointsY[0]), &(p->pointsY[lowest]));

    /* Initalize an array of the index for use in the stack. */
    int indexArr[p->numPoints];
    for (int i = 0; i < p->numPoints; i++) {
        indexArr[i] = i;
    }

    /* Sort the points based on their polar angles with respect to the lowest
     * point based on the merge sort algorithm. */
    mergeSort(p, indexArr, 1, p->numPoints - 1, s);

    /* Initialize an empty stack to store convex hull points. */
    struct stack *stack = createStack(&indexArr[0]);
    struct stack **stackPtr = &stack;

    /* Push the first three points to the stack. */
    for (int i = 1; i < 3; i++) {
        push(stackPtr, &indexArr[i]);
    }

    /* Iterate over the remaining points. */
    for (int i = 3; i < p->numPoints; i++) {
        /* While the current point and the two points below the top of the stack
         * make a non-left turn, pop the top of the stack. */
        while (orientation(p, *(int *)secondTop(stackPtr),
                           *(int *)top(stackPtr), indexArr[i]) != 2) {
            pop(stackPtr);
        }

        /* Push the current point to the stack. */
        push(stackPtr, &indexArr[i]);
    }
    
    /* The stack now contains the convex hull points. So we pop all items from
     * the stack and add them to the convex hull. */
    hull = newList();
    int current = 0;
    long double currentX = 0, currentY = 0;

    while (secondTop(stackPtr)) {
        current = *(int *)pop(stackPtr);
        currentX = p->pointsX[current];
        currentY = p->pointsY[current];
        insertTail(hull, currentX, currentY);
    }

    /* Since the last item of the stack should be the starting point of the
     * convex hull, we insert it to the head instead of the tail. */
    current = *(int *)pop(stackPtr);
    currentX = p->pointsX[current];
    currentY = p->pointsY[current];
    insertHead(hull, currentX, currentY);

    free(stack);

    s->convexHull = hull;
    
    return s;
}

int findLowest(long double *points1, long double *points2, int numPoints) {
    int minIndex = 0;

    for (int i = 1; i < numPoints; i++) {
        /* Selecting the lowest point among the points1 array. */
        if (points1[i] < points1[minIndex]) {
            minIndex = i;
            /* If there are multiple lowest points in the points1 array, choose
             * the one with the lowest coordinate in the corresponding points2
             * array. */
        } else if (points1[i] == points1[minIndex]) {
            if (points2[i] < points2[minIndex]) {
                minIndex = i;
            }
        }
    }

    return minIndex;
}

long double calcPolar(long double x1, long double y1, long double x2,
                      long double y2) {
    return atan2(y2 - y1, x2 - x1);
}

void swap(long double *a, long double *b) {
    long double temp = *a;
    *a = *b;
    *b = temp;
}

void merge(struct problem* p, int* arr, int left, int middle, int right,
           struct solution* s) {
    /* Maintain the index of the subarrays and the original array. */
    int indexLeft = 0, indexRight = 0, indexOri = 0;

    /* Create and initialize the two subarrays of the splitted original array
     * copying the data from the original array to the subarrays. */
    int numLeft = middle - left + 1;
    int numRight = right - middle;
    int leftArr[numLeft], rightArr[numRight];

    for (indexLeft = 0; indexLeft < numLeft; indexLeft++) {
        leftArr[indexLeft] = arr[left + indexLeft];
    }
    for (indexRight = 0; indexRight < numRight; indexRight++) {
        rightArr[indexRight] = arr[middle + indexRight + 1];
    }

    /* Merge the two subarrays into the original array. */
    indexLeft = 0;
    indexRight = 0;
    indexOri = left;

    double angleLeft = 0.0, angleRight = 0.0;
    /* Until we reach either the end of the left or right subarray, we compare
     * the elements between the two arrays and place them in the correct
     * position in the original array. */
    while (indexLeft < numLeft && indexRight < numRight) {
        /* Compute and compare the polar angles of the left and right subarrays.
         */
        angleLeft = calcPolar(p->pointsX[0], p->pointsY[0],
                              p->pointsX[leftArr[indexLeft]],
                              p->pointsY[leftArr[indexLeft]]);
        angleRight = calcPolar(p->pointsX[0], p->pointsY[0],
                               p->pointsX[rightArr[indexRight]],
                               p->pointsY[rightArr[indexRight]]);
        s->operationCount++;
        if (angleLeft < angleRight) {
            arr[indexOri] = leftArr[indexLeft];
            indexLeft++;
        } else if (angleLeft == angleRight) {
            if (p->pointsY[leftArr[indexLeft]] <
                p->pointsY[rightArr[indexRight]]) {
                arr[indexOri] = leftArr[indexLeft];
                indexLeft++;
            } else {
                arr[indexOri] = rightArr[indexRight];
                indexRight++;
            }
        } else {
            arr[indexOri] = rightArr[indexRight];
            indexRight++;
        }
        indexOri++;
    }

    /* When we reached the end of either one of the subarrays, we pick the
     * remaining element in the other array and put that in the last element of
     * the original array. */
    while (indexLeft < numLeft) {
        arr[indexOri] = leftArr[indexLeft];
        indexLeft++;
        indexOri++;
    }

    while (indexRight < numRight) {
        arr[indexOri] = rightArr[indexRight];
        indexRight++;
        indexOri++;
    }
}

void mergeSort(struct problem* p, int* arr, int left, int right,
               struct solution* s) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        /* Sort first and second halves. */
        mergeSort(p, arr, left, middle, s);
        mergeSort(p, arr, middle + 1, right, s);

        /* Merge the two halves together. */
        merge(p, arr, left, middle, right, s);
    }
}

void *top(struct stack **s) {
    /* Pop the first item in the stack and store that item. */
    void *item = pop(s);
    /* Then push it back into the stack to retain the state of the stack. */
    push(s, item);
    return item;
}

void *secondTop(struct stack **s) {
    /* Pop the first two item in the stack and store that item. */
    void *item1 = pop(s);
    void *item2 = pop(s);
    /* Then push them back into the stack to retain the state of the stack. */
    push(s, item2);
    push(s, item1);
    return item2;
}
