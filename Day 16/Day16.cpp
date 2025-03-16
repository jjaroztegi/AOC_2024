#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// Right, Down, Left, Up
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};
string dp[] = {">", "v", "<", "^"};

pair<int, int> findLetter(const vector<vector<string>> &map,
                          const string letter) {
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[0].size(); j++) {
            if (map[i][j] == letter) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

pair<vector<string>, int> dijkstra(const vector<vector<string>> &map,
                                   int startX, int startY, int endX, int endY) {
    int rows = map.size();
    int cols = map[0].size();

    vector<vector<int>> scoreMap(rows, vector<int>(cols, INT_MAX));
    vector<vector<pair<int, int>>> predecessorMap(
        rows, vector<pair<int, int>>(cols, {-1, -1}));
    vector<vector<int>> directionMap(rows, vector<int>(cols, -1));
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    struct Node {
        int score;
        int x;
        int y;
        int last_direction;
    };

    vector<Node> nodes;
    Node startNode = {0, startX, startY, -1};
    nodes.push_back(startNode);
    scoreMap[startX][startY] = 0;

    while (nodes.size() > 0) {
        // Find node with minimum score
        int min_idx = 0;
        int min_score = nodes[0].score;
        for (int i = 1; i < nodes.size(); i++) {
            if (nodes[i].score < min_score) {
                min_score = nodes[i].score;
                min_idx = i;
            }
        }

        int current_score = nodes[min_idx].score;
        int x = nodes[min_idx].x;
        int y = nodes[min_idx].y;
        int last_direction_index = nodes[min_idx].last_direction;

        // Remove the node at min_idx
        for (int i = min_idx; i < nodes.size() - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        nodes.pop_back();

        if (visited[x][y]) {
            continue;
        }
        visited[x][y] = true;

        if (x == endX && y == endY) {
            vector<string> path;
            int currentX = endX;
            int currentY = endY;
            while (currentX != startX || currentY != startY) {
                int dir_index = directionMap[currentX][currentY];
                path.push_back(dp[dir_index]);
                pair<int, int> prev = predecessorMap[currentX][currentY];
                currentX = prev.first;
                currentY = prev.second;
            }

            vector<string> reversed_path;
            for (int i = path.size() - 1; i >= 0; i--) {
                reversed_path.push_back(path[i]);
            }
            return {reversed_path, current_score};
        }

        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols &&
                map[newX][newY] != "#") {
                int new_step_score;
                if (predecessorMap[x][y].first == -1) { // Starting move
                    new_step_score = (i == 0) ? 1 : 1001;
                } else {
                    new_step_score = (i == last_direction_index) ? 1 : 1001;
                }
                int total_new_score = current_score + new_step_score;

                if (total_new_score < scoreMap[newX][newY]) {
                    scoreMap[newX][newY] = total_new_score;
                    predecessorMap[newX][newY] = {x, y};
                    directionMap[newX][newY] = i;
                    if (!visited[newX][newY]) {
                        Node newNode = {total_new_score, newX, newY, i};
                        nodes.push_back(newNode);
                    }
                }
            }
        }
    }
    return {{}, -1};
}

int main() {
    string filename = "input.txt";
    ifstream inputFile(filename);

    vector<vector<string>> map;
    string line;
    while (getline(inputFile, line)) {
        vector<string> row;
        for (size_t i = 0; i < line.length(); i++) {
            row.push_back(string(1, line[i]));
        }
        map.push_back(row);
    }

    auto [startX, startY] = findLetter(map, "S");
    auto [endX, endY] = findLetter(map, "E");

    auto [bestPath, lowestScore] = dijkstra(map, startX, startY, endX, endY);

    cout << "\nScore: " << lowestScore << endl;
    return 0;
}