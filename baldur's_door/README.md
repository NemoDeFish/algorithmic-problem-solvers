# 🛡️ Task 2: Baldur's Door

**Baldur's Door** is a fantasy puzzle-solving module inspired by classic role-playing games. In this multi-part task, you'll help a party of adventurers navigate poison-filled forests, disarm dangerous lairs, establish resource trade networks, and survive the legendary Mythic Delves.

Each part represents a different problem-solving scenario, requiring graph algorithms such as BFS, Dijkstra’s algorithm, and Minimum Spanning Tree (MST) computation.

---

## 🧭 Overview

Each part of the task is a standalone program that reads from an input file and prints a single result:

| Part | Description                       | Algorithm          | Output                       |
| ---- | --------------------------------- | ------------------ | ---------------------------- |
| A    | Find shortest path through poison | BFS                | Minimum number of steps      |
| B    | Find cheapest path through traps  | Dijkstra           | Minimum total disarm cost    |
| C    | Connect all materials cheaply     | Kruskal/Prim (MST) | Total cost of full network   |
| D    | Survive a mythic delve            | Modified Dijkstra  | Final % multiplier (floored) |

---

## 🧩 Parts A–D

### Part A – Poison Forest Escape (BFS)

**Goal:**
Help the rogue escape a dense poison-filled forest using the fewest possible steps.

**Input:**

* Unweighted undirected graph
* Damage is `1` per step

**Output:**

* Minimum steps (damage taken)

### Part B – Trap-Laden Lair (Dijkstra)

**Goal:**
Traverse a lair while disarming traps with minimal material cost.

**Input:**

* Weighted undirected graph
* Edge weights = trap disarm costs

**Output:**

* Minimum total cost to exit the lair

### Part C – Material Conversion Network (MST)

**Goal:**
Build a material conversion network such that every material can be reached from any other, at the lowest possible total cost.

**Input:**

* Weighted undirected graph (no specific source/target)
* Edge weights = artisan service costs

**Output:**

* Minimum cost to connect all materials

### Part D – Mythic Delve Multiplier (Modified Dijkstra)

**Goal:**
Traverse a mystical dungeon where damage increases by percentage multipliers at every step.

**Input:**

* Weighted undirected graph
* Edge weights = percent increases (e.g., `20` = +20%)

**Output:**

* Final damage multiplier as a whole number (floor of total % increase)

---

## ✅ Functions

All solutions are implemented in `graph.c` and expose the following functions:

```c
struct solution *solveWithUnweightedBFS(struct graph *g, int numLocations, int start, int end);
struct solution *solveWithDijkstraCost(struct graph *g, int start, int end);
struct solution *solveWithPrimMST(struct graph *g, int numLocations, int start);
struct solution *solveWithMultiplicativeDijkstra(struct graph *g, int start, int end);
```

Each function returns a pointer to a `struct solution`:

```c
struct solution {
    struct linkedList *convexHull;  // May be NULL
    int operationCount;             // Final answer (steps, cost, etc.)
};
```
---

## 📥 Input Format

Each input file follows a specific structure depending on the part:

### Part A Input:

```
<number_of_nodes>
<number_of_edges>
<start_node>
<end_node>
<edge1_node1> <edge1_node2>
...
```

### Part B Input:

```
<number_of_nodes>
<number_of_edges>
<start_node>
<end_node>
<edge1_node1> <edge1_node2> <cost>
...
```

### Part C Input:

```
<number_of_nodes>
<number_of_edges>
<irrelevant_line>
<edge1_node1> <edge1_node2> <cost>
...
```

### Part D Input:

```
<number_of_nodes>
<number_of_edges>
<start_node>
<end_node>
<edge1_node1> <edge1_node2> <percent_increase>
...
```

---

## 🧪 Testing

Test cases for each part are in the tests/ folder:

```
tests/
├── p2a-in-1.txt   # Input for Part A
├── p2a-out-1.txt  # Expected output for Part A
├── p2b-in-1.txt   # Input for Part B
├── p2b-out-1.txt  # ...
...
```

Each input file follows the format and corresponds to a specific graph problem:

| Part | File Pattern   | Problem Type             |
| ---- | -------------- | ------------------------ |
| A    | `p2a-in-*.txt` | Unweighted Shortest Path |
| B    | `p2b-in-*.txt` | Weighted Shortest Path   |
| C    | `p2c-in-*.txt` | Minimum Spanning Tree    |
| D    | `p2d-in-*.txt` | Multiplicative Path Cost |

You can write test scripts or use `diff` to verify outputs:

```bash
./partA test_cases/2a-in-1.txt > output.txt
diff output.txt test_cases/2a-out-1.txt
```

---

## 🏃 Usage

To run tests for any part, write a small C file that:

1. Parses the input in the expected format
2. Constructs the graph
3. Calls the appropriate solver function
4. Prints the value of `operationCount` in the returned solution

---

## 🛠 Compilation & Testing

To compile a test program for Part A:

```bash
gcc -o partA test_partA.c graph.c
```

To test output:

```bash
./partA > output.txt
diff output.txt tests/p2a-out-1.txt
```

Repeat similarly for other parts (B, C, D).