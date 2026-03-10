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

The approach taken consists of a simple and straightforward brute-force solution implemented using the standard libraries from C++. The implementation relies on a helper function that calculates the minimum number of coins required for a given amount, called repeatedly at each iteration.

### Input reading
The input, as declared previously, consists of a single line containing the number of test cases. The following lines consist of a single test case containing the maximum amount (in its range from 1 to the read number) in order to calculate the average and the maximum amount of coins required to pay, the number of coin denominations and then the coins.

<h4> Storage </h4>
All the test cases have been stored in a matrix. Additionally, we store in an array the N value of each test case.

### Solution

Starting off by calculating the maximum coin of the denomination set. By iterating through each amount, we set the local minimum value to infinite and an upper bound which is the maximum coin denomination plus the current amount. After that, we try all possible ways to pay that amount, calculating the minimum coins required to pay and to receive change, and updating the running minimum accordingly. After all combinations, we calculate the average (2 decimal places) and maximum number of coins exchanged.

<h4> Minimum of coins </h4>
The key of this problem is a helper function that calculates, for a given amount, the minimum number of coins required to cover it. In order to do so, we iterate through each value up to the amount and each coin denomination to check whether adding that coin leads to a better solution than what was previously found.

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

The approach uses a visibility-based depth assignment. Each letter is a component (command center). Components are grouped by letter, bounds and size are computed, then layers are assigned by comparing which component is visible more often in the overlap region.

### Input reading
The input consists of a single line with the number of ships. Each ship starts with its grid dimensions X and Y and a scale factor Z, followed by X rows of Y whitespace-separated characters. Any character outside `[a-zA-Z]` is treated as empty space.

<h4> Storage </h4>
Each ship stores its character grid and its scale factor. Components are derived on demand when the shooting sequence is requested.

### Solution

For each ship, all cells are scanned once to build a bounding box per distinct letter. The centroid is the center of the bounding box scaled by Z. The grid shows which component is visible at each cell — we use this to decide stacking order.

For each overlapping pair of components, we count how often each appears in the overlap region. The one visible more often is "in front" (closer to the viewer). We assign depth so each component is behind all that are in front of it, processing in rounds until all depths are set. Components are sorted by bounding box size (smallest first) for processing order. Within each layer, components are sorted by size then letter.

<h4> Depth layering </h4>
The key is using the grid to infer stacking: the component shown at a cell is in front at that position. For overlapping pairs, we compare visibility counts in the overlap rectangle to determine who is in front. Depths are then assigned so each component is behind all components that are in front of it.

<h4> Optimizations </h4>
- Each overlapping pair is checked once (skip when chA ≥ chB)
- Reserve capacity for the component order vector
- Move semantics when storing ships to avoid copying grids
