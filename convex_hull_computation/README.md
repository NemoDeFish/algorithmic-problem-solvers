# 📐 Convex Hull Project 

## Overview

This project implements two classic algorithms for computing the **Convex Hull** of a set of points in 2D space:

* **Jarvis’ March (Gift Wrapping)**
* **Graham’s Scan**

Both algorithms utilize a **doubly linked list** data structure to store and output the convex hull points efficiently.

---

## 📁 Repository Structure
```
.
├── convexHull.c           # Convex Hull algorithm implementations
├── convexHull.h           # Convex Hull module header with function declarations
├── linkedlist.c           # Doubly linked list implementation
├── linkedlist.h           # Doubly linked list interface
├── test_case/             # Input test files (CSV format)
├── test_case_answers/     # Expected output files for verification
├── README.md              # Project overview and instructions
```

---

## 🛠️ Features

* **Jarvis March** (`jarvisMarch`): Iteratively selects the next hull point by finding the largest counterclockwise angle.
* **Graham Scan** (`grahamScan`): Sorts points by polar angle and uses a stack-like doubly linked list to form the hull.
* Both algorithms return solutions as doubly linked lists for flexible traversal.
* Input points read from CSV files.
* Outputs convex hull points in both **clockwise** and **counterclockwise** orders.

---

## 📌 Functions You Can Use
```c
struct solution jarvisMarch(struct Point *points, int n);
```

> Computes the convex hull using Jarvis’ March algorithm.
> Returns a `struct solution` containing:
>
> * `convexHull`: a pointer to a doubly linked list of hull points.
> * `operationCount`: the number of angle comparisons performed.

```c
struct solution grahamScan(struct Point *points, int n);
```

> Computes the convex hull using Graham’s Scan algorithm.
> Returns a `struct solution` containing:
>
> * `convexHull`: a pointer to a doubly linked list of hull points.
> * `operationCount`: the number of angle comparisons performed during sorting.

Each function takes:

* `points`: an array of `struct Point` representing the input points.
* `n`: the number of points.

---

## 📥 Input Format

Input should be provided as a CSV file with the following format:

```
<number_of_points>
x1 y1
x2 y2
...
xn yn
```

* The first line contains the total number of points.
* Each subsequent line contains two space-separated floating-point values representing the x and y coordinates of a point.

---

## ▶️ Compiling and Running

Use any C compiler. Example using GCC:

```bash
gcc -o convexHull main.c convexHull.c linkedList.c
```

Your `main.c` should:

1. Load points from a CSV file.
2. Call either `jarvisMarch()` or `grahamScan()` with the loaded points.
3. Access the returned `struct solution` to:

   * Traverse the `convexHull` linked list to print points in clockwise and counterclockwise order.
   * Output the `operationCount` to report the number of angle comparisons performed.

Example run:

```bash
./convexHull input_points.csv
```

---

## 🔍 About the Algorithms

### Jarvis March

* Starts with the leftmost point.
* Selects the next hull point by choosing the one forming the largest counterclockwise angle.
* Time complexity: O(nh), where n = number of points, h = points on hull.

### Graham Scan

* Starts with the lowest point by y-coordinate.
* Sorts remaining points by polar angle relative to this point.
* Uses a stack (implemented with doubly linked list) to maintain hull points.
* Time complexity: O(n log n), due to sorting.

---

## 📝 Notes

* The doubly linked list allows easy traversal in both directions for outputting hull points.
* The code is modular, separating convex hull logic and linked list operations for clarity and reusability.