#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <iomanip>
#include <algorithm>

using namespace std;
pair<double, int> solution(vector<int> &c, int N);
int minCoins(vector<int> &coins, int amount);

int main(int argc, char *argv[]) {
  // Verify input file
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input file>" << endl;
    return 1;
  }

  // Open input file
  ifstream input(argv[1]);
  if (!input.is_open()) {
    cerr << "Error: could not open input file " << argv[1] << endl;
    return 1;
  }

  // Read number of test cases
  int num_cases;
  input >> num_cases;
  vector<vector<int>> cases(num_cases);
  vector<int> N_values(num_cases);

  // Read each test case
  for (int i = 0; i < num_cases; i++) {
    int N, K, coin;
    input >> N >> K;
    N_values[i] = N;
    cases[i].resize(K);
    for (int j = 0; j < K; j++) {
      input >> coin;
      cases[i][j] = coin;
    }
  }

  // Process and output results
  for (int i = 0; i < num_cases; i++) {
    pair<double, int> result = solution(cases[i], N_values[i]);
    cout << fixed << setprecision(2) << result.first << " " << result.second << endl;
  }

  input.close();

  // Write output file
  ofstream output("coins/output.txt");
  if (!output.is_open()) {
    cerr << "Error: could not open output file " << argv[2] << endl;
    return 1;
  }

  for (int i = 0; i < num_cases; i++) {
    pair<double, int> result = solution(cases[i], N_values[i]);
    output << fixed << setprecision(2) << result.first << " " << result.second << endl;
  }

  output.close();

  return 0;
}

// Helper function to compute minimum coins needed for a given amount
int minCoins(vector<int> &coins, int amount) {
  vector<int> aux(amount + 1, INT_MAX);
  aux[0] = 0;

  for (int i = 1; i <= amount; i++) {
    for (int coin : coins) {
      if (i >= coin && aux[i - coin] != INT_MAX) {
        aux[i] = min(aux[i], aux[i - coin] + 1);
      }
    }
  }
  return aux[amount];
}

pair<double, int> solution(vector<int> &c, int N) {
  int max_coin = *max_element(c.begin(), c.end());
  int total_coins = 0;
  int max_coins = 0;
  
  // For each amount X from 1 to N
  for (int X = 1; X <= N; X++) {
    int min_total = INT_MAX;
    
    // Try all possible Y >= X (you pay Y, cashier gives back Y-X)
    // Upper bound: N + max_coin to ensure we consider all optimal scenarios
    int upper_bound = N + max_coin;
    
    for (int Y = X; Y <= upper_bound; Y++) {
      int coins_paid = minCoins(c, Y);
      int coins_received = minCoins(c, Y - X);
      
      if (coins_paid != INT_MAX && coins_received != INT_MAX) {
        min_total = min(min_total, coins_paid + coins_received);
      }
    }
    
    if (min_total != INT_MAX) {
      total_coins += min_total;
      max_coins = max(max_coins, min_total);
    }
  }
  
  double average = (double)total_coins / N;
  return {average, max_coins};
}
