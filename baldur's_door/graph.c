/* graph.c
    Set of vertices and edges implementation.

    Implementations for helper functions for graph construction and manipulation.

    Implemented by Si Yong Lim
*/
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

struct solution * solveWithUnweightedBFS(struct graph * g, int numLocations, int start, int end);
struct solution * solveWithDijkstraCost(struct graph * g, int start, int end);
struct solution * solveWithPrimMST(struct graph * g, int numLocations, int start);
struct solution * solveWithMultiplicativeDijkstra(struct graph * g, int start, int end);

struct solution * solveWithUnweightedBFS(struct graph * g, int numLocations, int start, int end) {
    struct solution * solution =
        (struct solution * ) malloc(sizeof(struct solution));
    assert(solution);

    /* Mark each node in V with 0. */
    int vertices[numLocations], marked[numLocations],
        shortest[numLocations];
    for (int i = 0; i < numLocations; i++) {
        vertices[i] = i;
        marked[i] = 0;
        shortest[i] = 0;
    }

    /* Initialize queue */
    struct pq * queue = newPQ();
    int priority = 0;

    for (int v = startingLocation; v < numLocations; v++) {
        if (marked[v] == 0) {
            marked[v]++;
            enqueue(queue, & vertices[v], v);

            while (empty(queue) != 1) {
                int u = * (int * ) deletemin(queue);
                // Explore all neighbours of the current vertice
                for (int i = 0; i < g -> numEdges; i++) {
                    // Check if neighbouring vertice is not visited from
                    // start to end
                    if (g -> edgeList[i] -> start == u &&
                        marked[g -> edgeList[i] -> end] == 0) {
                        marked[g -> edgeList[i] -> end]++;
                        // Mark the distance of the neighbouring vertice as
                        // distance of the current vertice + 1
                        shortest[g -> edgeList[i] -> end] =
                            shortest[g -> edgeList[i] -> start] + 1;
                        // Insert the neighbouring vertice into the queue
                        enqueue(queue, & vertices[g -> edgeList[i] -> end],
                            priority++);
                    }

                    // Check if neighbouring vertice is not visited from end
                    // to start
                    if (g -> edgeList[i] -> end == u &&
                        marked[g -> edgeList[i] -> start] == 0) {
                        marked[g -> edgeList[i] -> start]++;
                        // Mark the distance of the neighbouring vertice as
                        // distance of the current vertice + 1
                        shortest[g -> edgeList[i] -> start] =
                            shortest[g -> edgeList[i] -> end] + 1;
                        // Insert the neighbouring vertice into the queue
                        enqueue(queue, & vertices[g -> edgeList[i] -> start],
                            priority++);
                    }
                }
            }
        }
    }

    freePQ(queue);

    solution -> damageTaken = shortest[finalLocation];
    return solution;
}

struct solution * solveWithDijkstraCost(struct graph * g, int numLocations, int start, int end) {
    struct solution * solution =
        (struct solution * ) malloc(sizeof(struct solution));
    assert(solution);
    // Crate an adjacency list
    struct list ** adjacencyList = createAdjList(g -> numVertices);
    populateAdjList(adjacencyList, g);

    // Initialise the set of vertices V
    int vertices[g -> numVertices];
    // Initialise the output array, which is the distance from the starting
    // location to the index i
    int dist[g -> numVertices];
    // Initialise a previous array
    int added[g -> numVertices];

    for (int v = 0; v < g -> numVertices; v++) {
        vertices[v] = v;
        dist[v] = INT_MAX;
        added[v] = -1;
    }

    dist[startingLocation] = 0;

    // Initialize the priority queue, where the priorities are the distances
    struct pq * queue = newPQ();
    for (int v = 0; v < g -> numVertices; v++) {
        enqueue(queue, & vertices[v], dist[v]);
    }

    /* Find the shortest path for all vertices. */
    while (empty(queue) != 1) {
        /* Pick the node with the minimum distance. */
        int u = * (int * ) deletemin(queue);
        /* Check if the node has already been popped from the queue before.
         * If it is not visited before, we mark it as visited by
         * incrementing it. If it has already been visited before, we just
         * continue. */
        if (added[u]++ != -1) {
            continue;
        }

        struct list * currentEdge = adjacencyList[u];
        /* Traverse the adjacency list of the popped vertex and check each
         * connections. */
        while (currentEdge) {
            struct edge * edge = (struct edge * ) deleteHead( & currentEdge);
            int w = edge -> end;
            int weightUW = edge -> cost;
            /* Check if the new path has a lower distance. */
            if (dist[u] + weightUW < dist[w]) {
                dist[w] = dist[u] + weightUW;
                enqueue(queue, & vertices[w], dist[w]);
            }
            free(edge);
        }
    }

    free(adjacencyList);
    freePQ(queue);

    solution -> totalCost = dist[finalLocation];
    return solution;
}

struct solution * solveWithPrimMST(struct graph * g, int numLocations, int start, int end) {
    struct solution * solution =
        (struct solution * ) malloc(sizeof(struct solution));
    assert(solution);
    // Crate an adjacency list
    struct list ** adjacencyList = createAdjList(g -> numVertices);
    populateAdjList(adjacencyList, g);

    /* Integer array to store the cost of going through each node. */
    int cost[numLocations];
    /* Boolean array to store whether the node has already been added to the
     * queue before. */
    bool added[numLocations];
    /* Integer array for usage of the priority queue to store the nodes. */
    int vertices[numLocations];

    for (int i = 0; i < numLocations; i++) {
        vertices[i] = i;
        cost[i] = INT_MAX;
        added[i] = false;
    }

    /* Set the starting location as a cost of 0 so that it is picked as the
     * first node. */
    int v0 = startingLocation;
    cost[v0] = 0;

    // Initialize the priority queue, where the priorities are the cost
    // values
    struct pq * queue = newPQ();
    for (int v = 0; v < g -> numVertices; v++) {
        enqueue(queue, & vertices[v], cost[v]);
    }

    while (empty(queue) != 1) {
        /* Pick the node with the minimum cost. */
        int u = * (int * ) deletemin(queue);
        /* Check if the node has already been popped from the queue before.
         * If it is not visited before, we mark it as visited by
         * incrementing it. If it has already been visited before, we just
         * continue. */
        if (added[u] != false) {
            continue;
        }

        struct list * currentEdge = adjacencyList[u];
        /* Traverse the adjacency list of the popped vertex and check each
         * connections. */
        while (currentEdge) {
            struct edge * edge = (struct edge * ) deleteHead( & currentEdge);
            int w = edge -> end;
            int weightUW = edge -> cost;

            /* Check if the current node can be reached by a smaller
             * distance. */
            if (weightUW < cost[w] && added[w] == false) {
                cost[w] = weightUW;
                enqueue(queue, & vertices[w], cost[w]);
            }
            free(edge);
        }
        /* Mark the vertex as visited by the priority queue. */
        added[u] = true;
    }

    // Calculate the number of elements in the array
    int n = sizeof(cost) / sizeof(cost[0]);
    int artisanCost = 0;

    // Iterate through the array and add each element to the sum
    for (int i = 0; i < n; i++) {
        artisanCost += cost[i];
    }

    free(adjacencyList);
    freePQ(queue);

    solution -> artisanCost = artisanCost;
    return solution;
}

struct solution * solveWithMultiplicativeDijkstra(struct graph * g, int numLocations, int start, int end) {
    struct solution * solution =
        (struct solution * ) malloc(sizeof(struct solution));
    assert(solution);
    // Crate an adjacency list
    struct list ** adjacencyList = createAdjList(g -> numVertices);
    populateAdjList(adjacencyList, g);

    // Initialise the set of vertices V
    int vertices[g -> numVertices];
    // Initialise the output array, which is the distance from the starting
    // location to the index i
    double dist[g -> numVertices];
    // Initialise a previous array to mark whether each vertex was visited
    // by the priority queue.
    int added[g -> numVertices];

    for (int v = 0; v < g -> numVertices; v++) {
        vertices[v] = v;
        dist[v] = INT_MAX;
        added[v] = -1;
    }

    /* Start from 1 as we need to calculate in percentages. */
    dist[startingLocation] = 1;

    // Initialize the priority queue, where the priorities are the distances
    struct pq * queue = newPQ();
    for (int v = 0; v < g -> numVertices; v++) {
        enqueue(queue, & vertices[v], dist[v]);
    }

    while (empty(queue) != 1) {
        int u = * (int * ) deletemin(queue);
        /* Check if the node has already been popped from the queue before.
         */
        if (added[u]++ != -1) {
            continue;
        }

        struct list * currentEdge = adjacencyList[u];
        /* Traverse the adjacency list of the popped vertex and check each
         * connections. */
        while (currentEdge) {
            struct edge * edge = (struct edge * ) deleteHead( & currentEdge);
            int w = edge -> end;
            /* Convert the percentage to a value that can be multiplied. */
            double weightUW = 1.0 + (double)(edge -> cost) / 100;

            /* If the current multipier is lower the previous multiplier, we
             * update the priority queue. */
            if (dist[u] * weightUW < dist[w]) {
                dist[w] = dist[u] * weightUW;
                /* Priority here is multiplied by 100 because it is stored
                 * as an integer. Otherwise, it will just truncate to 1,
                 * essentially leaving all the nodes at the same priority.
                 */
                enqueue(queue, & vertices[w], dist[w] * 100);
            }
            free(edge);
        }
    }

    free(adjacencyList);
    freePQ(queue);

    solution -> totalPercentage = (dist[finalLocation] - 1) * 100;
    return solution;
}

/* Create an adjacency list */
struct list ** createAdjList(int numVertices) {
    struct list ** adjacencyLists = malloc(numVertices * sizeof(struct list * ));

    int i;
    for (i = 0; i < numVertices; i++) adjacencyLists[i] = NULL;

    return adjacencyLists;
}

/* Populate the adjacency list with edges*/
void populateAdjList(struct list ** adjacencyList, struct graph * g) {
    for (int i = 0; i < g -> numEdges; i++) {
        // Create a new edge structure to hold destination vertex and cost
        struct edge * edgeEnd = (struct edge * ) malloc(sizeof(struct edge));
        assert(edgeEnd);
        edgeEnd -> end = g -> edgeList[i] -> end;
        edgeEnd -> cost = g -> edgeList[i] -> cost;

        // Add edge from A to B
        adjacencyList[g -> edgeList[i] -> start] =
            prependList(adjacencyList[g -> edgeList[i] -> start], edgeEnd);

        // Create a new edge structure to hold destination vertex and cost
        struct edge * edgeStart = (struct edge * ) malloc(sizeof(struct edge));
        assert(edgeStart);
        edgeStart -> end = g -> edgeList[i] -> start;
        edgeStart -> cost = g -> edgeList[i] -> cost;

        // Add edge from B to A
        adjacencyList[g -> edgeList[i] -> end] =
            prependList(adjacencyList[g -> edgeList[i] -> end], edgeStart);
    }
}