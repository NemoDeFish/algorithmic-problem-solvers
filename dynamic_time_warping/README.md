# ⏱️ Dynamic Time Warping (DTW) Toolkit

## Overview

This project implements a suite of **Dynamic Time Warping (DTW)** algorithms designed to measure and analyze similarity between temporal sequences that vary in timing or speed. DTW is a powerful method widely used in domains such as speech processing, finance, healthcare, and activity recognition to align sequences that differ in length or pacing.

The toolkit includes standard DTW as well as advanced variants with practical constraints to improve performance and alignment quality.

---

## 📂 Project Structure

```
/
├── test_cases/                 # Input test data sequences
├── test_cases_answers/         # Expected outputs for verification
├── dynamic_time_warping.h      # Implementation of core DTW functions
├── dynamic_time_warping.h      # Header with core DTW function declarations
```

---

## ⚙️ Key Functions

The primary interface exposes three DTW implementations:

* **`dynamicTimeWarping(struct problem *p)`**
  Standard DTW algorithm that computes the full cost matrix and returns the optimal alignment cost and matrix.

* **`dynamicTimeWarpingWithWindow(struct problem *p)`**
  DTW with boundary constraints limiting alignment to a diagonal window (`window_size`), enhancing efficiency and preventing unrealistic warping.

* **`dynamicTimeWarpingWithPathConstraint(struct problem *p)`**
  DTW with a constraint on the total path length (`max_path_length`), allowing adaptive control over the alignment complexity and flexibility.

Each function returns a `struct solution` containing the alignment cost and related data.

---

## ✨ Features and Benefits

* **Flexible Alignment:** Align sequences with variations in speed, length, and timing.
* **Performance Optimization:** Window constraints reduce unnecessary computations.
* **Adaptive Complexity:** Path length constraints allow balancing alignment precision and computational cost.
* **Modular Design:** Easily extendable for additional constraints or sequence types.
* **Comprehensive Testing:** Organized test cases and expected answers for validation.

---

## 🛠️ How It Works

DTW calculates the minimal cumulative cost of aligning two sequences by allowing insertions, deletions, and matches with flexible timing. The boundary window restricts alignment paths to near-diagonal cells, while the path constraint caps the total number of alignment steps.

---

## 📋 Usage

1. Place your input sequences in the `test_cases/` folder.
2. Use the corresponding expected results in `test_cases_answers/` for verification.
3. Include `dynamic_time_warping.h` and link with the implementation source files.
4. Call the desired DTW function with your `problem` struct input.

---

## 📊 Computational Complexity

| Algorithm                       | Time Complexity              |
| ------------------------------- | ---------------------------- |
| Standard DTW                    | O(n × m)                     |
| DTW with Boundary Window        | O(n × window\_size)          |
| DTW with Path Length Constraint | O(n × m × max\_path\_length) |

Where `n` and `m` are sequence lengths.