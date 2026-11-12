# Algorithm Battle: "Which Data Structure Wins?"
## University Data Structures & Algorithms Assignment (CLA – 2)
**Application Domain:** Student Record Search Engine  
**Course Regulations:** B.Tech. 2021 Regulations — SRM Institute of Science and Technology  
**Language Standard:** C++17 (`-std=c++17 -Wall -Wextra -O2`)

---

## Table of Contents
1. [Project Overview & File Architecture](#project-overview--file-architecture)
2. [Compilation & Execution Guide](#compilation--execution-guide)
3. [Part A: Problem Selection](#part-a-problem-selection)
4. [Part B: Implementation Overview](#part-b-implementation-overview)
5. [Part C: Experimental Setup & Results (Empirical Data)](#part-c-experimental-setup--results-empirical-data)
6. [Part D: Algorithm Battle Comparison Table](#part-d-algorithm-battle-comparison-table)
7. [Part E: Final Verdict & Technical Recommendation](#part-e-final-verdict--technical-recommendation)
8. [Technical Defense / Viva Voce Cheat-Sheet](#technical-defense--viva-voce-cheat-sheet)

---

## Project Overview & File Architecture

This project is an empirical comparative evaluation of three fundamental data structures—**Binary Search Tree (BST)**, **Self-Balancing AVL Tree**, and **Hash Table with Open Addressing (Linear Probing)**—implemented to solve the real-world problem of indexing and searching university **Student Records**.

The workspace is modularized into human-readable, decoupled header and source files:

```
c:\Users\Lenovo\Documents\DTM ASSIGNMENT\
├── Student.h          # Student record entity struct and formatted console display
├── BST.h              # Unbalanced Binary Search Tree (insert, search, traversal, height)
├── AVL.h              # Self-balancing AVL Tree (rotations, balance factor, insert, search)
├── HashTable.h        # Hash Table (m=53 prime slots, linear probing, collision counter)
├── dataset.csv        # 40 realistic student records with partially sorted keys
├── main.cpp           # CLI benchmark runner (--benchmark) & Interactive Viva Menu
└── README.md          # Comprehensive technical report and empirical analysis
```

---

## Compilation & Execution Guide

The project is written in standard C++17 with zero third-party dependencies.

### 1. Compilation
Compile using GCC (`g++`):
```bash
g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o student_search
```

### 2. Automated Benchmark Execution
Run the automated experimental benchmark on the 40-record dataset:
```bash
./student_search --benchmark
```

### 3. Interactive Viva Mode
Launch the interactive CLI menu for live faculty testing (insertion, single-key searching, traversal, and hash layout inspection):
```bash
./student_search
```

---

## Part A: Problem Selection

### 1. What is the Problem?
In modern university academic management systems (e.g., student portals, ERP platforms, exam administration systems), the institution must store, retrieve, and update thousands of student profile records. Each student is uniquely identified by an integer **Registration Number (`regNo`)**, associated with metadata:
- Student Full Name (`name`)
- Academic Department (`dept`)
- Cumulative Grade Point Average (`cgpa`)

### 2. Why is Efficient Searching Required?
During campus placement drives, semester grade publications, fee clearance checks, and course registrations, the portal processes tens of thousands of concurrent read queries.
- A naive linear search across an unordered array or linked list requires $\mathcal{O}(N)$ key comparisons per search. For $N = 50{,}000$ students, an unsuccessful search checks 50,000 records, creating server latency and database bottlenecks.
- Low-latency lookups ($\mathcal{O}(1)$ or $\mathcal{O}(\log N)$) are essential to maintain responsiveness under heavy workloads.

### 3. Why are BST, AVL, and Hashing Suitable for Comparison?
These three data structures represent distinct paradigms in computer science for solving the key-value dictionary problem:
1. **Unbalanced BST:** Provides hierarchical, order-preserving storage. Simple to implement with dynamic memory, but vulnerable to structural degeneration ($\mathcal{O}(N)$ worst-case) when inputs arrive in sequential or partially sorted order (e.g., chronological registration numbers).
2. **AVL Tree:** Enforces a strict height balance invariant ($|h_L - h_R| \le 1$) via tree rotations. It guarantees worst-case $\mathcal{O}(\log N)$ lookups regardless of key insertion order, while preserving sorted order for range queries and in-order traversals.
3. **Hash Table (Linear Probing):** Uses direct indexing via a hash function ($h(k) = k \pmod M$) with open addressing. It aims for average $\mathcal{O}(1)$ constant-time lookup, but faces performance degradation due to **primary clustering** when the load factor $\alpha = N/M$ is high.

Evaluating them on the exact same dataset highlights their trade-offs in search latency, insertion overhead, memory footprint, and worst-case guarantees.

---

## Part B: Implementation Overview

### 1. Data Structure Architectures

#### A. Binary Search Tree (`BST.h`)
- **Node Structure:** `BSTNode` storing `Student data`, `BSTNode* left`, and `BSTNode* right`.
- **Search Logic:** Compares target `regNo` with current node's `regNo`. Traverses left if `target < node.regNo`, else right.
- **Comparison Counting:** Increments `comparisons` counter on every visited node.
- **Memory Management:** Recursive post-order traversal destructor `destroyTree()` prevents memory leaks.

#### B. Self-Balancing AVL Tree (`AVL.h`)
- **Node Structure:** `AVLNode` storing `Student data`, child pointers, and an integer `height`.
- **Balance Factor:** Calculated as $BF(\text{node}) = \text{height}(\text{left}) - \text{height}(\text{right})$.
- **Rotation Mechanics:**
  - **Left-Left (LL) Imbalance ($BF > 1, \text{key} < \text{left.key}$):** Single Right Rotation (`rightRotate`).
  - **Right-Right (RR) Imbalance ($BF < -1, \text{key} > \text{right.key}$):** Single Left Rotation (`leftRotate`).
  - **Left-Right (LR) Imbalance ($BF > 1, \text{key} > \text{left.key}$):** Left Rotate left child, then Right Rotate node.
  - **Right-Left (RL) Imbalance ($BF < -1, \text{key} < \text{right.key}$):** Right Rotate right child, then Left Rotate node.
- **Search Logic:** Identical to BST ($\mathcal{O}(\log N)$ guaranteed depth).

#### C. Hash Table with Linear Probing (`HashTable.h`)
- **Fixed Size ($M$):** $M = 53$ (a prime number to optimize distribution for $N=40$ records).
- **Hash Function:** $h(\text{regNo}) = \text{regNo} \pmod{53}$.
- **Collision Resolution:** Open Addressing via Linear Probing:
  $$\text{index}_i = (h(\text{regNo}) + i) \pmod{53}, \quad i = 0, 1, 2, \dots$$
- **Collision Metric:** Increments `totalCollisions` whenever the initial hash slot $h(\text{regNo})$ is already occupied.
- **Search Comparison Counting:** Increments `comparisons` on each inspected table slot until either the target record is found or an empty slot (`isOccupied == false`) is encountered.

### 2. Theoretical Complexity Summary

| Data Structure | Insertion (Average) | Insertion (Worst) | Search (Average) | Search (Worst) | Space Complexity | Ordered Traversal? |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **BST** | $\mathcal{O}(\log N)$ | $\mathcal{O}(N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(N)$ | $\mathcal{O}(N)$ | Yes ($\mathcal{O}(N)$) |
| **AVL Tree** | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(N)$ | Yes ($\mathcal{O}(N)$) |
| **Hash Table (Linear Probe)** | $\mathcal{O}(1)$ | $\mathcal{O}(N)$ | $\mathcal{O}(1)$ | $\mathcal{O}(N)$ | $\mathcal{O}(M)$ | No ($\mathcal{O}(M \log M)$ to sort) |

---

## Part C: Experimental Setup & Results (Empirical Data)

### 1. Dataset Configuration
- **File:** `dataset.csv`
- **Total Records ($N$):** 40 unique student entries.
- **Key Pattern:** Realistic university registration numbers ranging from `1001` to `1080`.
- **Ordering Characteristic:** Partially sorted (clustered sequential admissions with periodic local inversions). This reflects realistic college registration sequences where numbers increase chronologically, triggering skew in naive BSTs while testing AVL rebalancing.

### 2. Structural Metrics After 40 Insertions
These are the empirical metrics produced by running `./student_search --benchmark`:

- **Unbalanced BST Height:** **30**  
  *(Due to partially ascending registration keys, the BST suffered severe skew, creating a path 30 nodes deep for a 40-node dataset).*
- **Balanced AVL Tree Height:** **6**  
  *(Maintained strict balance: $\lceil\log_2(40 + 1)\rceil = 6$, confirming optimal $\mathcal{O}(\log N)$ depth).*
- **Hash Table Total Initial Collisions:** **11**
- **Hash Table Total Insertion Probe Steps:** **43**
- **Hash Table Capacity:** **53 slots**
- **Hash Table Final Load Factor ($\alpha$):** $40 / 53 = \mathbf{75.47\%}$

---

### 3. Test 1: 5 Successful Searches (Records Present in Dataset)

| RegNo | Student Name | Department | CGPA | BST Comparisons | AVL Comparisons | Hash Table Comparisons | Search Characteristics |
| :---: | :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **1001** | Aarav Sharma | CSE | 8.85 | 1 | 5 | 1 | Root of BST; direct hash hit ($1001 \pmod{53} = 47$) |
| **1025** | Varun Joshi | EEE | 8.15 | 9 | 1 | 1 | Root of AVL after rotations; direct hash hit ($1025 \pmod{53} = 18$) |
| **1042** | Aniket Ghosh | ECE | 8.65 | 16 | 5 | 1 | Intermediate BST depth; direct hash hit ($1042 \pmod{53} = 35$) |
| **1068** | Deepa Natarajan | CSE | 8.95 | 26 | 5 | 6 | Deep in skewed BST; collided in Hash Table (+5 probe steps) |
| **1080** | Bhavana Varma | CSE | 9.25 | 30 | 6 | 3 | Deepest leaf in BST; collided in Hash Table (+2 probe steps) |
| **AVG** | **—** | **—** | **—** | **16.40** | **4.40** | **2.40** | **Hash Table is fastest on average** |

---

### 4. Test 2: 5 Unsuccessful Searches (Records Absent from Dataset)

| RegNo | Test Reason / Scenario | BST Comparisons | AVL Comparisons | Hash Table Comparisons | Empirical Behavioral Analysis |
| :---: | :--- | :---: | :---: | :---: | :--- |
| **999** | Below minimum key range ($< 1001$) | 1 | 5 | 2 | BST exits immediately on left child of root; Hash Table probes 2 slots |
| **1004** | Missing intermediate key (between 1003 & 1005) | 3 | 5 | 23 | Primary clustering in Hash Table forces scanning through 23 contiguous occupied slots |
| **1020** | Missing intermediate key (between 1019 & 1022) | 9 | 5 | 7 | AVL strictly bounded; Hash Table probes cluster |
| **1054** | Missing key colliding with slot 47 ($1054 \pmod{53} = 47$) | 21 | 6 | 26 | Severe primary cluster traversal in Hash Table (26 probes to hit empty slot) |
| **2000** | Above maximum key range ($> 1080$) | 30 | 6 | 1 | BST traverses full skewed height (30 checks); Hash Table hits empty slot 39 instantly |
| **AVG** | **—** | **12.80** | **5.40** | **11.80** | **AVL Tree wins on unsuccessful searches** |

---

### 5. Overall Comparisons Summary

| Evaluation Metric | Binary Search Tree (BST) | Self-Balancing AVL Tree | Hash Table (Linear Probing) |
| :--- | :---: | :---: | :---: |
| **Average Successful Search Comparisons** | 16.40 | 4.40 | **2.40** (Fastest) |
| **Average Unsuccessful Search Comparisons** | 12.80 | **5.40** (Fastest) | 11.80 |
| **Combined Overall Search Comparisons** | 14.60 | **4.90** (Most Consistent) | 7.10 |
| **Worst-Case Search Comparisons Observed** | 30 | **6** | 26 |
| **Best-Case Search Comparisons Observed** | 1 | 1 | 1 |
| **Structure Height** | 30 | **6** | N/A (Flat Array) |
| **Collisions Encountered** | N/A | N/A | 11 collisions (43 probes) |

---

## Part D: Algorithm Battle Comparison Table

*(Structured exactly in accordance with the SRM Institute CLA – 2 Evaluation Rubric)*

| Parameter | Binary Search Tree (BST) | AVL Tree | Hashing (Linear Probing) |
| :--- | :--- | :--- | :--- |
| **Search Comparisons** | **Poor & Unstable:** Average 16.40 for hits, 12.80 for misses. Worst-case required 30 comparisons due to rightward skew. | **Optimal & Stable:** Average 4.40 for hits, 5.40 for misses. Worst-case strictly capped at 6 comparisons ($\approx \log_2 N$). | **Bimodal:** Extremely fast for hits without collision (average 2.40 comparisons), but degraded severely on misses (up to 26 comparisons) due to clustering. |
| **Insertion Observations** | **Fast insertion ($\mathcal{O}(H)$), but deteriorates:** No rebalancing overhead, but subsequent insertions get slower as the tree grows deeper. | **Higher insertion overhead:** Requires height recalculation and tree rotations (LL, RR, LR, RL), but guarantees logarithmic depth. | **Fast until high load factor:** Fast constant-time placement initially, but 11 collisions occurred; high load factor ($\alpha = 75.5\%$) led to 43 probe steps. |
| **Height** | **Height = 30** (Severe skew for 40 nodes; theoretical minimum is 6). | **Height = 6** (Strictly balanced; $\lceil \log_2(41) \rceil = 6$). | **N/A** (Contiguous flat array of 53 slots). |
| **Collisions** | **N/A** (Tree structure handles duplicates at nodes). | **N/A** (Tree structure handles duplicates at nodes). | **11 Initial Collisions** (Total 43 linear probe steps taken across 40 insertions). |
| **Overall Observation** | Unsuitable for sequential student registration numbers due to tree degeneration towards an $\mathcal{O}(N)$ linked list. | **Most reliable general-purpose structure:** Guarantees predictable $\mathcal{O}(\log N)$ lookups, resists skew, and maintains full sorted ordering. | **Best for pure point-lookups** when table size is generously provisioned ($\alpha < 0.5$). Suffers from primary clustering at $\alpha > 0.7$. |

---

## Part E: Final Verdict & Technical Recommendation

### 1. Which Data Structure is the Winner?
The **AVL Tree** is declared the **Overall Champion** for the university **Student Record Search** application, with **Hashing** as an honorable mention for pure point-queries under lower load factors.

### 2. Empirical & Theoretical Justification

#### Why BST Loses:
- In real academic environments, student registration numbers are assigned sequentially (e.g., `1001, 1002, 1003...`).
- When sequential or partially sorted keys are inserted, the unbalanced BST degenerates into a linear chain.
- In our experiment, the BST reached a height of **30** for only 40 records. Searching for student `1080` required **30 comparisons**—equivalent to a slow linear search.

#### Why Hash Table is Runner-Up (With Caveats):
- Hash Table achieved the lowest comparison count for successful searches (**2.40 comparisons on average**), with 3 out of 5 tests requiring only **1 comparison**.
- However, at a load factor of **75.47%**, Linear Probing suffered heavily from **primary clustering** (where occupied slots form contiguous blocks).
- For unsuccessful searches, the Hash Table averaged **11.80 comparisons**, and searching for absent key `1054` required **26 linear probes** before an empty slot was reached.
- Furthermore, Hash Tables cannot support alphabetical or numerical roster generation (in-order traversals) without an auxiliary sorting step ($\mathcal{O}(M \log M)$).

#### Why AVL Tree Wins:
1. **Strict Worst-Case Guarantees:** Regardless of whether registration numbers arrive sorted, reverse-sorted, or random, AVL rotations maintain the height at **6**.
2. **Predictable Comparison Counts:** AVL searches never exceeded **6 comparisons** in any test case (average 4.40 for successful, 5.40 for unsuccessful searches).
3. **Dual Capability (Search + Range Queries):** Unlike Hash Tables, AVL trees support effortless In-Order Traversal ($\mathcal{O}(N)$), allowing the registrar to generate sorted student rosters, merit lists, and department-wise rank lists without extra memory or sorting algorithms.
4. **No Clustering Penalties:** AVL performance does not degrade when load exceeds 70%, avoiding the rehashing and table-doubling penalties required by Hash Tables.

---

## Technical Defense / Viva Voce Cheat-Sheet

During your viva voce defense, the examiner will test your understanding of how each data structure behaves when a live key is supplied. Use this cheat-sheet:

### 1. What happens when a new key (e.g., `1085`) is inserted?
- **In BST:** It traverses down the right spine (since $1085 > 1080$), attaching as the right child of `1080`. BST height increases from **30 to 31**.
- **In AVL Tree:** Traverses to the leaf, inserts the node, and checks balance factors backward up the call stack. If any ancestor has $|BF| > 1$, it executes a single or double rotation, maintaining height at **6 or 7**.
- **In Hash Table:** Calculates $1085 \pmod{53} = 25$.
  - Checking the layout table: slot 25 is currently **EMPTY**.
  - `1085` is placed directly at index 25 with **0 collisions** and **0 probe offset**.

### 2. What is Primary Clustering in Linear Probing?
- When multiple keys hash to adjacent slots, they create a continuous block of occupied cells.
- Any subsequent key that hashes anywhere into or near this block must probe through the entire block to find an open slot, making clusters grow even larger.
- This explains why searching for absent key `1054` required **26 comparisons** in our benchmark.

### 3. How do the AVL Rotations work?
- **Single Rotations (LL and RR):** Executed when the inserted key is in the "outer" subtree (Left-of-Left or Right-of-Right). Requires 3 pointer adjustments.
- **Double Rotations (LR and RL):** Executed when the inserted key is in the "inner" subtree (Right-of-Left or Left-of-Right). First rotates the child outward, then rotates the root inward.

### 4. How to run live tests during viva?
Launch the interactive CLI menu:
```bash
./student_search
```
- Select Option `1` to search any live key given by the examiner and display step-by-step comparisons across BST, AVL, and Hash Table.
- Select Option `2` to insert the examiner's test record and immediately view the structural height nchanges and collision events.
- Select Option `4` to display the entire 53-slot Hash Table layout, showing original hash indices and probe offsets.

