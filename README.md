# 🧠 Algorithmic Problem Solvers

Welcome to my portfolio repository showcasing four algorithmic problem-solving modules. Each project demonstrates skills in data structures, graph theory, dynamic programming, and computational geometry — all implemented in C and tested using structured input files.

---

## 📂 Projects Overview

| Project Folder                                          | Description                                                                                    | Technologies Used                             |
| ------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | --------------------------------------------- |
| [`baldur's_door`](./baldur's_door/)                     | A fantasy-themed collection of graph problems: shortest paths, MSTs, and multiplicative costs. | C, Graphs (BFS, Dijkstra, Prim), File Parsing |
| [`boggle_buddy`](./boggle_buddy/)                       | A Boggle word solver using DFS and trie-based backtracking.                                    | C, DFS, Trie Structures                       |
| [`convex_hull_computation`](./convex_hull_computation/) | Computes convex hulls using Graham Scan and Gift Wrapping algorithms.                          | C, Computational Geometry                     |
| [`dynamic_time_warping`](./dynamic_time_warping/)       | Calculates sequence similarity using Dynamic Time Warping.                                     | C, Dynamic Programming                        |

---

## 📁 Repository Structure

```
.
├── baldur's_door/
├── boggle_buddy/
├── convex_hull_computation/
├── dynamic_time_warping/
└── README.md
```

---

## 🛠 Usage

Each folder contains C source files implementing core logic. To run a particular algorithm, create your own `main.c` file that:

1. Reads the test input from the appropriate file.
2. Builds the required data structures (e.g. graph or point set).
3. Calls the appropriate function.
4. Prints or checks the output.

Each function returns a pointer to a `struct solution`.

---

## 🧪 Testing

Each project contains its own `test_cases/` directory with input files and expected output files in `test_cases_answers/`:

```
project_name/
├── test_cases/
│   ├── p2a-in-1.txt      # Sample input file
├── test_cases_answers/
│   └── p2a-out-1.txt     # Expected output file
```

Refer to the `README.md` inside each folder for input format details and example execution.
