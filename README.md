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

## Approach taken

The approach taken consists of a simple and straight forward (brute force) solution implemented using the standard libraries from C++. The implementation, constists of an helper function that calculates the minimum amount of coins required per each amount at each iteration.

### Input reading
The input, as declared previously, consists of a single line containing the number of test cases. The following lines consists of a single test case containing the maximum amount (in its range fro m1 to the read number) in order to calculate the average and the maximum amount of coins required to pay, the number of coin denominations and then then the coins.

<h4> Storage </h4>
All the test cases have been stored in a matrix. Additionally, we store in an array the N value of each test case.

### Solution

Starting off by calculating the maximum coin of the denomination set. By iterating through each amount, we set the local minimum value to infinite and an upper bound which is the maximum coin denomination plus the amount to calculate. After that, we try all possible ways to reach teh amount, calculating the minimum amount of coins to pay and to receive and updating the minimum coins value from infinite to the calculated value. After all combinations, we calculate average (2 decimal places) and maximum number of coins exchanged.

<h4> Minimum of coins </h4>
The key of this problem is based on a helper functions to calculate per each ammount, the minimum number of coins required to cover the specified amount.

In order to so, we iterate through the amount and each coin denomination to check if the current coin or the previous coin is enough to satisfy the current amount.
