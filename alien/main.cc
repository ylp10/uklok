#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

void assert_bounds(char ch, int value, int min, int max);
void assert_regex(char ch, char &to_store_in);

struct Entry {
  char name;
  int area;
  double cx, cy;
  double px, py;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input file>" << endl;
    return 1;
  }

  ifstream input(argv[1]);
  if (!input.is_open()) {
    cerr << "Error: could not open input file " << argv[1] << endl;
    return 1;
  }

  int N;
  input >> N;
  assert_bounds('N', N, 1, 30);
  int X, Y;
  double Z;
  vector<double> Z_values;

  vector<vector<vector<char>>> ships(N);
  for (int i = 0; i < N; i++) {
    input >> X >> Y >> Z;
    assert_bounds('X', X, 4, 100);
    assert_bounds('Y', Y, 4, 100);
    Z_values.push_back(Z);
    ships[i].resize(X);
    for (int j = 0; j < X; j++) {
      ships[i][j].resize(Y);
      for (int k = 0; k < Y; k++) {
        char ch;
        input >> ch;
        assert_regex(ch, ships[i][j][k]);
      }
    }
  }

  for (int s = 0; s < N; s++) {
    const auto &grid = ships[s];
    const int rows = grid.size();
    const int cols = grid[0].size();
    const double scale = Z_values[s];

    // One entry per letter: aggregate all cells of that letter
    map<char, pair<double, double>> sum_rc;  // sum row, sum col
    map<char, int> count;

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        char c = grid[i][j];
        if (c == ' ') continue;
        sum_rc[c].first += i;
        sum_rc[c].second += j;
        count[c]++;
      }
    }

    vector<Entry> entries;
    for (const auto &p : count) {
      char name = p.first;
      int area = p.second;
      double cx = sum_rc[name].first / area;
      double cy = sum_rc[name].second / area;
      double px = (cx + 0.5) * scale;
      double py = (cy + 0.5) * scale;
      entries.push_back({name, area, cx, cy, px, py});
    }

    // Group by depth = floor(cy). Layer order: (i+1)%n. Within layer: area ascending, then name (lexicographical).
    map<int, vector<Entry>> by_depth;
    for (const auto &e : entries)
      by_depth[(int)e.cy].push_back(e);

    vector<int> depths;
    for (const auto &p : by_depth) depths.push_back(p.first);
    sort(depths.begin(), depths.end());
    int num_layers = (int)depths.size();
    map<int, int> depth_to_key;
    for (int i = 0; i < num_layers; i++)
      depth_to_key[depths[i]] = (i + 1) % num_layers;

    vector<pair<int, vector<Entry>>> layers;
    for (const auto &p : by_depth) {
      vector<Entry> v = p.second;
      sort(v.begin(), v.end(), [](const Entry &a, const Entry &b) {
        if (a.area != b.area) return a.area < b.area;
        return a.name < b.name;
      });
      layers.push_back({depth_to_key[p.first], std::move(v)});
    }
    sort(layers.begin(), layers.end(), [](const auto &a, const auto &b) {
      return a.first < b.first;
    });

    bool first_layer = true;
    for (const auto &p : layers) {
      if (!first_layer) cout << " ";
      first_layer = false;
      for (size_t i = 0; i < p.second.size(); i++) {
        if (i > 0) cout << ";";
        const Entry &e = p.second[i];
        cout << e.name << ":" << fixed << setprecision(3) << e.px << "," << e.py;
      }
    }
    cout << endl;
  }

  return 0;
}

void assert_bounds(char ch, int value, int min, int max) {
  if (value < min || value > max) {
    cerr << "Value " << ch << " = " << value << " is out of bounds: [" << min << ", " << max << "]" << endl;
    exit(1);
  }
}

void assert_regex(char ch, char &to_store_in) {
  if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
    to_store_in = ' ';
  } else {
    to_store_in = ch;
  }
}
