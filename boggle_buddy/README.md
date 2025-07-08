# 🧠 Boggle Buddy

**Boggle Buddy** is a fast and flexible C program that solves Boggle boards using a **prefix trie** and **graph traversal**. It can:

* 🔍 Find all valid words on a Boggle board.
* 💡 Provide auto-completion hints for a partial word.
* 🏆 Find the longest valid word on the board.

Perfect for learners, language games, or fast offline play!

---

## ✨ Features

* ✅ Prefix Trie for efficient word lookup
* ✅ Board traversal using DFS with adjacency rules
* ✅ Optional hint mode for incomplete word stems
* ✅ Option to restrict repeated use of letters (not just positions)
* ✅ Clean, modular C code with reusable components

---

## 📁 Project Structure

```
.
├── boggle.h / boggle.c         # Core Boggle solver logic
├── prefixTree.h / prefixTree.c # Prefix trie for dictionary
├── test_cases/                 # Directory for example test cases
├── test_cases_answers          # Directory for example test cases answers
├── README.md                   
```

---

## 🛠 How It Works

### 🔡 Dictionary Trie

Implemented as a 256-way prefix trie (1 slot for each ASCII character). Each node marks if a full word ends there. This allows fast prefix pruning during board traversal.

### 🎲 Board Graph

4x4 grid of characters, connected to adjacent neighbors (vertical, horizontal, diagonal). Solving uses DFS from each cell, matching characters with trie edges.

---

## 📌 Functions You Can Use
```c
struct solution *findAllValidWords(struct problem *p);
```

> Returns all dictionary-valid words that can be made from the board.

```c
struct solution *autocompletePartialWord(struct problem *p);
```

> Given a partial word, returns the list of next valid characters that could extend it (e.g., for hints).

```c
struct solution *findLongestValidWord(struct problem *p);
```

> Returns the longest valid word on the board that follows Boggle rules.

Each function takes a `struct problem *p`, which encapsulates:

* The loaded dictionary trie
* The board letters
* Any additional options (e.g., repeated-letter rule)

---

## 📥 Input Format

### 🔤 Dictionary File

Example (`dictionary.txt`):

```
10
cleaner
clean
lean
opera
open
pants
panel
pager
speak
team
```

* First line: Number of words
* One word per line (lowercase)

### 🧩 Board File

Example (`board.txt`):

```
O E E T
S P A G
J N E L
V E R C
```

* 4 lines, 4 space-separated uppercase letters
* Case-insensitive internally

---

## ▶️ Usage

This section shows how to integrate and run the Dynamic Time Warping algorithms from this project.

### 🔧 Including the Library

Include the header in your C source file:

```c
#include "dynamic_time_warping.h"
```

### 🧩 Preparing Input

Define and populate a `struct problem` instance that holds your input sequences and any parameters:

* Sequences to align (e.g., arrays of floats or ints)
* Lengths of each sequence
* Parameters like `window_size` or `max_path_length` if using constrained variants

### ⚙️ Calling the Functions

Use one of the following functions depending on your needs:

```c
struct solution *dynamicTimeWarping(struct problem *p);
struct solution *dynamicTimeWarpingWithWindow(struct problem *p);
struct solution *dynamicTimeWarpingWithPathConstraint(struct problem *p);
```

Each function returns a pointer to a `struct solution` containing:

* The DTW cost (alignment cost)
* The DTW matrix (when applicable)
* Additional alignment info


### ▶️ Compiling and Running

Use any C compiler. Example using GCC:

```bash
gcc -o boggle main.c boggle.c prefixTree.c
./boggle dictionary.txt board.txt
```

The `main.c` file should construct the `problem` struct, load the board and dictionary, and call the desired function(s). Output can then be printed or processed.


### 🧪 Testing

* Place your input sequences as files in `test_cases/`.
* Store expected outputs in `test_cases_answers/`.
* Use your own test harness or integrate with automated tests to verify correctness.

---

## 📊 Sample Output (for `findAllValidWords`)

Given the example files, the program might output:

```
clean
cleaner
lean
open
pager
panel
```

---

## 🧠 Complexity Discussion
In the original version, the worst-case time complexity is **O(n² × 8ᴸ)** where:

* *n²* is the number of starting positions on the board,
* *8* is the maximum number of directions (adjacency),
* *L* is the maximum word length.

This exponential growth arises because each cell can recursively explore up to 8 neighbors for each letter in a word.

When we introduce the **unique-letter constraint**, where no letter may appear more than once in a word (regardless of its position on the board), the complexity improves significantly. Now, the recursion depth is bounded by the size of the alphabet *S* (i.e., number of unique letters), not by word length.

This reduces the upper bound to **O(n² × S)**:

* *n²* still accounts for board positions,
* *S* limits both the recursion depth and the branching factor, as repeated letters are skipped.

In boards with many repeated letters, this constraint sharply cuts down unnecessary paths, making the algorithm both faster and more efficient in practice.
