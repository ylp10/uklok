# UKLOK's code challenges

## 1. Optimal exchange

> Input

```c++
M
N K [...]
.
.
.

// M = number of cases
// N = max amount of each test case
// K = number of coin denominations
// [...] = the array of K coins denominations
```

> Output

```c++
// For each test case
avg max_coins
// avg = average number of coins to pay a X amount in interval [1, N]
// max_coins = max number of coins required to pay a X amount
```

## Approach taken

The approach taken consists of a graph-based optimization implemented using the standard libraries from C++. The implementation consists of a single-pass coin-change DP that, viewed as a BFS on an integer graph, computes the minimum coins needed for every amount in one sweep, avoiding redundant recalculations.

### Input reading
The input, as declared previously, consists of a single line containing the number of test cases. The following lines consist of a single test case containing the maximum amount (in its range from 1 to the read number) in order to calculate the average and the maximum amount of coins required to pay, the number of coin denominations and then the coins.

<h4> Storage </h4>
All the test cases have been stored in a matrix. Additionally, we store in an array the N value of each test case.

### Solution

Starting off by computing the distance table: each integer from 0 to N + max_coin is treated as a node in a directed graph, with an edge of weight 1 from node v to node v + coin for each denomination. A single left-to-right scan fills the minimum-coins distance from node 0 to every node in O(M × K).

After that, for each purchase amount X we scan all possible overpay amounts Y to find the pair (Y, Y–X) that minimizes the total coins exchanged, using direct lookups into the precomputed table. We then calculate the average (2 decimal places) and maximum number of coins exchanged.

<h4> Minimum of coins </h4>
The key of this problem is replacing repeated calls to a helper function with a single precomputed distance table. Each entry dist[v] is filled by checking all coin denominations: if we can already reach v – coin, we can reach v by adding one more coin. We keep the smallest result across all denominations.

---

## 2. Alien ships

> Input

```c++
N
X Y Z
[X lines of Y space-separated characters]
.
.
.

// N = number of ships (1 <= N <= 30)
// X = grid height, Y = grid width (4 <= X,Y <= 100)
// Z = scale factor for real-world coordinates
// [a-zA-Z] = command center cell; anything else = empty space
```

> Output

```c++
// For each ship, one line:
(W:Px,Py[;W:Px,Py])+ separated by blanks per depth layer
// W   = command center letter
// Px  = real X centroid (3 decimal places)
// Py  = real Y centroid (3 decimal places)
// layers ordered by depth, entries within a layer separated by ';'
// within a layer: sorted ascending by area then alphabetical order
```

## Approach taken

The approach taken consists of a bounding-box overlap graph coloring implemented using the standard libraries from C++. The implementation groups all cells sharing the same letter into a single component regardless of connectivity, then assigns depth layers using a greedy algorithm on the resulting overlap graph.

### Input reading
The input consists of a single line with the number of ships. Each ship starts with its grid dimensions X and Y and a scale factor Z, followed by X rows of Y whitespace-separated characters. Any character outside `[a-zA-Z]` is treated as empty space.

<h4> Storage </h4>
Each ship stores its character grid and its scale factor. Components are derived on demand when the shooting sequence is requested, so no extra per-cell storage is needed beyond the grid itself.

### Solution

For each ship, all cells are scanned once to build a bounding box per distinct letter: the smallest rectangle enclosing all occurrences of that letter. The centroid of each component is the center of its bounding box scaled by Z, and its area is the cell count multiplied by Z².

Depth layers are then assigned greedily. Components are processed in ascending order of bounding box area (alphabetical tiebreak). Each component receives the smallest depth not already taken by any overlapping component. Two components overlap if their bounding boxes intersect. Finally, components are grouped by depth, sorted within each group by area then letter, and formatted into the output string.

<h4> Depth layering </h4>
The key of this problem is treating bounding box overlaps as a graph coloring problem. Each letter is a node; two nodes share an edge if their bounding boxes overlap. Greedy coloring on this graph, with nodes ordered by bounding box size, assigns the minimum number of depth layers needed to separate all conflicting components.
