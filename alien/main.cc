#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

class Ship {
private:
  struct Component {
    char letter;
    int cell_count = 0;
    double cell_area;      // cell_count * Z^2, for within-depth sort
    int bounding_box_area; //  w*h, for depth processing order
    int minRow, maxRow, minCol, maxCol;
    double px, py; // center of bounding box * Z
    int depth = 0;
  };

  vector<vector<char>> grid;
  double scale;

  bool boxOverlaps(const Component &a, const Component &b) const {
    return a.minRow <= b.maxRow && b.minRow <= a.maxRow &&
           a.minCol <= b.maxCol && b.minCol <= a.maxCol;
  }

public:
  Ship() : scale(0) {}

  static vector<Ship> readFromFile(const string &filename) {
    ifstream input(filename);
    if (!input.is_open()) {
      cerr << "Error: could not open " << filename << endl;
      exit(1);
    }

    int n;
    input >> n;
    assert_bounds('N', n, 1, 30);

    vector<Ship> ships;
    for (int i = 0; i < n; i++) {
      int X, Y;
      double Z;
      input >> X >> Y >> Z;
      assert_bounds('X', X, 4, 100);
      assert_bounds('Y', Y, 4, 100);

      Ship ship;
      ship.scale = Z;
      ship.grid.resize(X, vector<char>(Y));
      for (int j = 0; j < X; j++)
        for (int k = 0; k < Y; k++) {
          char ch;
          input >> ch;
          assert_regex(ch, ship.grid[j][k]);
        }
      ships.push_back(ship);
    }
    input.close();
    return ships;
  }

  string getShootingSequence() const {
    int rows = grid.size(), cols = grid[0].size();

    // Group all cells by letter, build bounding boxes
    map<char, Component> compMap;
    for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++) {
        char ch = grid[r][c];
        if (ch == ' ')
          continue;
        if (!compMap.count(ch))
          compMap[ch] = {ch, 0, 0, 0, r, r, c, c, 0};
        Component &comp = compMap[ch];
        comp.cell_count++;
        comp.minRow = min(comp.minRow, r);
        comp.maxRow = max(comp.maxRow, r);
        comp.minCol = min(comp.minCol, c);
        comp.maxCol = max(comp.maxCol, c);
      }

    // Compute areas and centroid from bounding box center
    for (auto &[ch, comp] : compMap) {
      comp.cell_area = comp.cell_count * scale * scale;
      comp.bounding_box_area =
          (comp.maxRow - comp.minRow + 1) * (comp.maxCol - comp.minCol + 1);
      comp.px = (comp.minRow + comp.maxRow + 1) / 2.0 * scale;
      comp.py = (comp.minCol + comp.maxCol + 1) / 2.0 * scale;
    }

    // Assign each letter a depth layer
    vector<Component *> order;
    for (auto &[ch, comp] : compMap)
      order.push_back(&comp);
    sort(order.begin(), order.end(),
         [](const Component *a, const Component *b) {
           return a->bounding_box_area != b->bounding_box_area
                      ? a->bounding_box_area < b->bounding_box_area
                      : a->letter < b->letter;
         });
    for (Component *comp : order) {
      set<int> used;
      for (auto &[ch, other] : compMap)
        if (other.depth > 0 && boxOverlaps(*comp, other))
          used.insert(other.depth);
      int d = 1;
      while (used.count(d))
        d++;
      comp->depth = d;
    }

    // Group by depth, sort within depth by (cell_area ASC, letter ASC), format
    map<int, vector<const Component *>> byDepth;
    for (const auto &[ch, comp] : compMap)
      byDepth[comp.depth].push_back(&comp);

    ostringstream out;
    bool firstDepth = true;
    for (auto &[depth, comps] : byDepth) {
      sort(comps.begin(), comps.end(),
           [](const Component *a, const Component *b) {
             return a->cell_area != b->cell_area ? a->cell_area < b->cell_area
                                                 : a->letter < b->letter;
           });
      if (!firstDepth)
        out << " ";
      firstDepth = false;
      for (int i = 0; i < (int)comps.size(); i++) {
        if (i > 0)
          out << ";";
        out << comps[i]->letter << ":" << fixed << setprecision(3)
            << comps[i]->px << "," << comps[i]->py;
      }
    }
    return out.str();
  }

private:
  static void assert_bounds(char ch, int value, int min, int max) {
    if (value < min || value > max) {
      cerr << "Value " << ch << " = " << value << " out of bounds [" << min
           << "," << max << "]" << endl;
      exit(1);
    }
  }

  static void assert_regex(char ch, char &to_store_in) {
    to_store_in =
        ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) ? ch : ' ';
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input file>" << endl;
    return 1;
  }
  vector<Ship> ships = Ship::readFromFile(argv[1]);
  int start = clock();
  for (const auto &ship : ships)
    cout << ship.getShootingSequence() << endl;
  int end = clock();
  cerr << fixed << setprecision(4)
       << "Time taken: " << ((double)(end - start)) / CLOCKS_PER_SEC
       << " seconds" << endl;
  return 0;
}
