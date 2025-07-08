/*
    Implemented by Lim Si Yong

    Header for module which contains convex hull 
    specification data structures and functions.
*/

#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "linkedList.h"

/* Perform Jarvis' March to construct a convex
    hull for the given problem. */
struct solution *jarvisMarch(struct problem *p);

/* Perform Graham's Scan to construct a convex
    hull for the given problem. */
struct solution *grahamScan(struct problem *p);

#endif

