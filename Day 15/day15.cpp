#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Direction {
  private:
    static int dx[];
    static int dy[];

  public:
    vector<int> get(const string &dir) {
        int idx = 0;
        if (dir == ">")
            idx = 0;
        else if (dir == "v")
            idx = 1;
        else if (dir == "<")
            idx = 2;
        else if (dir == "^")
            idx = 3;
        return {dx[idx], dy[idx]};
    }
};

// Right, Down, Left, Up
int Direction::dx[] = {0, 1, 0, -1};
int Direction::dy[] = {1, 0, -1, 0};

class Map {
  private:
    vector<vector<string>> grid;

  public:
    Map(const vector<vector<string>> &initial_grid) : grid(initial_grid) {}

    vector<vector<string>> &getGrid() { return grid; }
    const vector<vector<string>> &getGrid() const { return grid; }

    void saveToFile(ofstream &output_file) const {
        for (size_t i = 0; i < grid.size(); i++) {
            for (size_t j = 0; j < grid[0].size(); j++) {
                output_file << grid[i][j];
            }
            output_file << endl;
        }
    }

    int sumCoordinates(const string &target) const {
        int sum = 0;
        for (size_t i = 0; i < grid.size(); i++) {
            for (size_t j = 0; j < grid[0].size(); j++) {
                if (grid[i][j] == target) {
                    sum += 100 * i + j;
                }
            }
        }
        return sum;
    }
    // Each cell is expanded horizontally
    Map createExpandedMap() const {
        int rows = grid.size();
        int cols = grid[0].size();
        vector<vector<string>> expanded(rows, vector<string>(2 * cols, "."));

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == "#") {
                    expanded[i][2 * j] = "#";
                    expanded[i][2 * j + 1] = "#";
                } else if (grid[i][j] == "O") {
                    expanded[i][2 * j] = "[";
                    expanded[i][2 * j + 1] = "]";
                } else {
                    expanded[i][2 * j] = grid[i][j];
                }
            }
        }
        return Map(expanded);
    }

    pair<int, int> findRobot() const {
        for (size_t i = 0; i < grid.size(); i++) {
            for (size_t j = 0; j < grid[0].size(); j++) {
                if (grid[i][j] == "@") {
                    return {i, j};
                }
            }
        }
        return {-1, -1};
    }
};

class Robot {
  private:
    int x, y;
    Direction dir;

    bool moveBoxesVertically(vector<vector<string>> &grid, int currX, int currY,
                             const vector<int> &direction) {
        int nextX = currX + direction[0];
        int nextY = currY;

        bool targetEmpty =
            (grid[nextX][nextY] == "." && grid[nextX][nextY + 1] == ".");
        bool targetIsBox =
            (grid[nextX][nextY] == "[" && grid[nextX][nextY + 1] == "]");
        bool targetIsLeftBox =
            (grid[nextX][nextY] == "]" && grid[nextX][nextY + 1] == ".");
        bool targetIsRightBox =
            (grid[nextX][nextY] == "." && grid[nextX][nextY + 1] == "[");
        bool targetIsDoubleBox =
            (grid[nextX][nextY] == "]" && grid[nextX][nextY + 1] == "[");

        if (targetEmpty) {
            grid[nextX][nextY] = "[";
            grid[nextX][nextY + 1] = "]";
            grid[currX][currY] = ".";
            grid[currX][currY + 1] = ".";
            return true;
        }

        if (targetIsBox && moveBoxesVertically(grid, nextX, nextY, direction)) {
            grid[nextX][nextY] = "[";
            grid[nextX][nextY + 1] = "]";
            grid[currX][currY] = ".";
            grid[currX][currY + 1] = ".";
            return true;
        }

        if (targetIsDoubleBox) {
            // Copy to be able to rollback
            vector<vector<string>> gridCopy = grid;

            if (!moveBoxesVertically(gridCopy, nextX, nextY - 1, direction)) {
                return false;
            }
            if (!moveBoxesVertically(gridCopy, nextX, nextY + 1, direction)) {
                return false;
            }

            // Apply
            grid = gridCopy;

            grid[nextX][nextY] = "[";
            grid[nextX][nextY + 1] = "]";
            grid[currX][currY] = ".";
            grid[currX][currY + 1] = ".";
            return true;
        }

        if (targetIsLeftBox) {
            if (!moveBoxesVertically(grid, nextX, nextY - 1, direction)) {
                return false;
            }
            grid[nextX][nextY] = "[";
            grid[nextX][nextY + 1] = "]";
            grid[currX][currY] = ".";
            grid[currX][currY + 1] = ".";
            return true;
        }

        if (targetIsRightBox) {
            if (!moveBoxesVertically(grid, nextX, nextY + 1, direction)) {
                return false;
            }
            grid[nextX][nextY] = "[";
            grid[nextX][nextY + 1] = "]";
            grid[currX][currY] = ".";
            grid[currX][currY + 1] = ".";
            return true;
        }

        return false;
    }

    void handleHorizontalMove(vector<vector<string>> &grid,
                              const vector<int> &dir, int &tempX, int &tempY,
                              const string &dirStr) {
        while (grid[tempX][tempY] == "[" || grid[tempX][tempY] == "]") {
            tempX += 2 * dir[0];
            tempY += 2 * dir[1];
        }

        if (grid[tempX][tempY] == ".") {
            grid[tempX][tempY] = (dirStr == ">") ? "]" : "[";

            int dy = tempY - y;
            int direction = 0;

            for (size_t j = 2; j < abs(dy); j++) {
                int newY = y + j * dir[1];
                string symbol = (direction + j) % 2 == 0
                                    ? (dirStr == ">" ? "[" : "]")
                                    : (dirStr == ">" ? "]" : "[");
                grid[x][newY] = symbol;
            }

            grid[x][y] = ".";
            grid[x + dir[0]][y + dir[1]] = "@";
            x += dir[0];
            y += dir[1];
        }
    }

    void handleVerticalMove(vector<vector<string>> &grid,
                            const vector<int> &direction, int &tempX,
                            int &tempY, const string &dirStr) {
        int boxY = tempY;

        if (grid[tempX][tempY] == "]") {
            boxY -= 1; // Start of box
        } else if (grid[tempX][tempY] != "[") {
            if (grid[tempX][tempY] == ".") {
                grid[x][y] = ".";
                grid[x + direction[0]][y + direction[1]] = "@";
                x += direction[0];
                y += direction[1];
            }
            return;
        }

        if (moveBoxesVertically(grid, tempX, boxY, direction)) {
            grid[x][y] = ".";
            grid[x + direction[0]][y + direction[1]] = "@";
            x += direction[0];
            y += direction[1];
        }
    }

  public:
    Robot(int startX, int startY) : x(startX), y(startY) {}

    pair<int, int> getPosition() const { return {x, y}; }

    void move(Map &map, const string &dirStr) {
        vector<vector<string>> &grid = map.getGrid();
        vector<int> direction = dir.get(dirStr);

        int tempX = x + direction[0];
        int tempY = y + direction[1];

        if (grid[tempX][tempY] == ".") {
            grid[tempX][tempY] = "@";
            grid[x][y] = ".";
            x = tempX;
            y = tempY;
        } else if (grid[tempX][tempY] == "#") {
            // Hit a wall - do nothing
        } else if (grid[tempX][tempY] == "O") {
            int boxX = tempX;
            int boxY = tempY;

            while (grid[boxX][boxY] == "O") {
                boxX += direction[0];
                boxY += direction[1];
            }

            if (grid[boxX][boxY] == ".") {
                grid[boxX][boxY] = "O";
                grid[x][y] = ".";
                grid[tempX][tempY] = "@";
                x = tempX;
                y = tempY;
            }
        }
    }

    void movePart2(Map &map, const string &dirStr) {
        vector<vector<string>> &grid = map.getGrid();
        vector<int> direction = dir.get(dirStr);

        int tempX = x + direction[0];
        int tempY = y + direction[1];

        if (grid[tempX][tempY] == ".") {
            grid[tempX][tempY] = "@";
            grid[x][y] = ".";
            x = tempX;
            y = tempY;
        } else if (grid[tempX][tempY] == "#") {
            // Hit a wall - do nothing
        } else if (grid[tempX][tempY] == "[" || grid[tempX][tempY] == "]") {
            if (dirStr == ">" || dirStr == "<") {
                handleHorizontalMove(grid, direction, tempX, tempY, dirStr);
            } else if (dirStr == "v" || dirStr == "^") {
                handleVerticalMove(grid, direction, tempX, tempY, dirStr);
            }
        }

        // Debug output
        // ofstream debugFile("debug.txt");
        // map.saveToFile(debugFile);
        // debugFile.close();
    }

    void executeAllMoves(Map &map, const vector<string> &directions,
                         bool isPart2 = false) {
        for (size_t i = 0; i < directions.size(); i++) {
            if (isPart2) {
                movePart2(map, directions[i]);
            } else {
                move(map, directions[i]);
            }
        }
    }
};

class GameSystem {
  private:
    vector<vector<string>> initialGrid;
    vector<string> directions;

  public:
    GameSystem(const string &filename) { loadFromFile(filename); }

    void loadFromFile(const string &filename) {
        ifstream inputFile(filename);
        string line;
        bool isMap = true;

        while (getline(inputFile, line)) {
            if (line.empty()) {
                isMap = false;
                continue;
            }

            if (isMap) {
                vector<string> row;
                for (size_t i = 0; i < line.length(); i++) {
                    row.push_back(string(1, line[i]));
                }
                initialGrid.push_back(row);
            } else {
                for (size_t i = 0; i < line.length(); i++) {
                    directions.push_back(string(1, line[i]));
                }
            }
        }
    }

    void runPart1(const string &outputFilename) {
        Map map(initialGrid);
        auto [startX, startY] = map.findRobot();
        Robot robot(startX, startY);

        robot.executeAllMoves(map, directions);

        ofstream outputFile(outputFilename);
        map.saveToFile(outputFile);
        outputFile.close();

        int sum = map.sumCoordinates("O");
        cout << "Part 1 - Sum of boxes' GPS coordinates: " << sum << endl;
    }

    void runPart2(const string &outputFilename) {
        Map initialMap(initialGrid);
        Map expandedMap = initialMap.createExpandedMap();

        // Debug output
        // ofstream expandedOutputFile("expanded_map.txt");
        // expandedMap.saveToFile(expandedOutputFile);
        // expandedOutputFile.close();

        auto [startX, startY] = expandedMap.findRobot();
        Robot robot(startX, startY);

        robot.executeAllMoves(expandedMap, directions, true);

        ofstream outputFile(outputFilename);
        expandedMap.saveToFile(outputFile);
        outputFile.close();

        int sum = expandedMap.sumCoordinates("[");
        cout << "Part 2 - Sum of boxes' GPS coordinates: " << sum << endl;
    }
};

int main() {
    GameSystem game("example.txt");
    game.runPart1("output.txt");
    game.runPart2("output_part2.txt");
    return 0;
}
