#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <ctime>

using namespace std;

pair<double, int> solution(vector<int> &c, int N);

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
  int start = clock();
  for (int i = 0; i < num_cases; i++) {
    pair<double, int> result = solution(cases[i], N_values[i]);
    cout << fixed << setprecision(2) << result.first << " " << result.second << endl;
  }
  int end = clock();
  cout << "Time taken: " << ((double)(end - start)) / CLOCKS_PER_SEC << " seconds" << endl;


  input.close();

  return 0;
}

pair<double, int> solution(vector<int> &c, int N) {
  const int max_coin = *max_element(c.begin(), c.end());
  const int M = N + max_coin;

  vector<int> dist(M + 1, INT_MAX);
  dist[0] = 0;
  for (int v = 1; v <= M; v++)
    for (int coin : c)
      if (v >= coin && dist[v - coin] != INT_MAX)
        // If we can make (v - coin), we can make v by adding this one coin
        dist[v] = min(dist[v], dist[v - coin] + 1);

  long long total_coins = 0;
  int max_coins = 0;

  // For each purchase amount X, find min coins over all (pay Y, receive Y-X)
  for (int X = 1; X <= N; X++) {
    int best = INT_MAX;
    for (int j = X; j <= M; j++) {
      // j = amount paid, j-X = change received; minimize total coins exchanged
      if (dist[j] != INT_MAX && dist[j - X] != INT_MAX)
        best = min(best, dist[j] + dist[j - X]);
    }
    if (best != INT_MAX) {
      total_coins += best;
      max_coins = max(max_coins, best);
    }
  }

  return {(double)total_coins / N, max_coins};
}

