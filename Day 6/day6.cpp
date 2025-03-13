#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<int> find_guard(vector<vector<string>> matrix) {
    vector<int> guard_location;
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == "^") {
                guard_location.push_back(i);
                guard_location.push_back(j);
            }
        }
    }
    return guard_location;
}

vector<vector<string>> draw_route(vector<vector<string>> matrix,
                                  vector<int> guard_location) {
    int row = guard_location[0];
    int col = guard_location[1];

    while (true) {
        // up
        while (matrix[row][col] != "#") {
            matrix[row][col] = "X";
            row--;
            if (matrix[row][col] != "#" && row == 0) {
                matrix[row][col] = "X";
                return matrix;
            }
        }
        row++;

        // right
        while (matrix[row][col] != "#") {
            matrix[row][col] = "X";
            col++;
            if (matrix[row][col] != "#" && col == matrix[0].size() - 1) {
                matrix[row][col] = "X";
                return matrix;
            }
        }
        col--;

        // down
        while (matrix[row][col] != "#") {
            matrix[row][col] = "X";
            row++;
            if (matrix[row][col] != "#" && row == matrix.size() - 1) {
                matrix[row][col] = "X";
                return matrix;
            }
        }
        row--;

        // left
        while (matrix[row][col] != "#") {
            matrix[row][col] = "X";
            col--;
            if (matrix[row][col] != "#" && col == 0) {
                matrix[row][col] = "X";
                return matrix;
            }
        }
        col++;
    }
}

int count_X(vector<vector<string>> matrix) {
    int count = 0;
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == "X") {
                count++;
            }
        }
    }
    return count;
}

vector<vector<string>> draw_loop(vector<vector<string>> matrix,
                                 vector<int> guard_location) {
    int row = guard_location[0];
    int col = guard_location[1];

    while (true) {
        // up
        while (matrix[row][col] != "#") {
            if (matrix[row][col] == "-") {
                matrix[row][col] = "+";
            } else {
                matrix[row][col] = "|";
            }
            row--;
            if (matrix[row][col] != "#" && row == 0) {
                if (matrix[row][col] == "-") {
                    matrix[row][col] = "+";
                } else {
                    matrix[row][col] = "|";
                }
                return matrix;
            }
        }
        row++;

        // right
        while (matrix[row][col] != "#") {
            if (matrix[row][col] == "|") {
                matrix[row][col] = "+";
            } else {
                matrix[row][col] = "-";
            }
            col++;
            if (matrix[row][col] != "#" && col == matrix[0].size() - 1) {
                if (matrix[row][col] == "|") {
                    matrix[row][col] = "+";
                } else {
                    matrix[row][col] = "-";
                }
                return matrix;
            }
        }
        col--;

        // down
        while (matrix[row][col] != "#") {
            if (matrix[row][col] == "-") {
                matrix[row][col] = "+";
            } else {
                matrix[row][col] = "|";
            }
            row++;
            if (matrix[row][col] != "#" && row == matrix.size() - 1) {
                if (matrix[row][col] == "-") {
                    matrix[row][col] = "+";
                } else {
                    matrix[row][col] = "|";
                }
                return matrix;
            }
        }
        row--;

        // left
        while (matrix[row][col] != "#") {
            if (matrix[row][col] == "|") {
                matrix[row][col] = "+";
            } else {
                matrix[row][col] = "-";
            }
            col--;
            if (matrix[row][col] != "#" && col == 0) {
                if (matrix[row][col] == "|") {
                    matrix[row][col] = "+";
                } else {
                    matrix[row][col] = "-";
                }
                return matrix;
            }
        }
        col++;
    }
}

string appendDirection(string current, string direction) {
    if (current.find(direction) == string::npos) {
        return current + direction;
    }
    return current;
}

int detect_loop(vector<vector<string>> matrix, vector<int> guard_location) {
    int loop = 0;

    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (i == guard_location[0] && j == guard_location[1]) {
                continue;
            }

            if (matrix[i][j] == "#") {
                continue;
            }

            // place obstruction
            vector<vector<string>> new_matrix = matrix;
            new_matrix[i][j] = "#";

            int row = guard_location[0];
            int col = guard_location[1];
            bool is_loop = false;
            bool exit_while = false;

            while (true) {
                // up
                while (new_matrix[row][col] != "#") {
                    if (new_matrix[row][col].find("∧") != string::npos) {
                        is_loop = true;
                        break;
                    }
                    if (new_matrix[row][col].find(">") != string::npos ||
                        new_matrix[row][col].find("<") != string::npos) {
                        new_matrix[row][col] =
                            appendDirection(new_matrix[row][col], "∧");
                    } else {
                        new_matrix[row][col] = "∧";
                    }
                    row--;
                    if (new_matrix[row][col] != "#" && row == 0) {
                        if (new_matrix[row][col].find(">") != string::npos ||
                            new_matrix[row][col].find("<") != string::npos) {
                            new_matrix[row][col] =
                                appendDirection(new_matrix[row][col], "∧");
                        } else {
                            new_matrix[row][col] = "∧";
                        }
                        exit_while = true;
                        break;
                    }
                }
                row++;
                if (is_loop || exit_while)
                    break;

                // right
                while (col < new_matrix[0].size() - 1 &&
                       new_matrix[row][col] != "#") {
                    if (new_matrix[row][col].find(">") != string::npos) {
                        is_loop = true;
                        break;
                    }
                    if (new_matrix[row][col].find("∧") != string::npos ||
                        new_matrix[row][col].find("v") != string::npos) {
                        new_matrix[row][col] =
                            appendDirection(new_matrix[row][col], ">");
                    } else {
                        new_matrix[row][col] = ">";
                    }
                    col++;
                    if (new_matrix[row][col] != "#" &&
                        col == new_matrix[0].size() - 1) {
                        if (new_matrix[row][col].find("∧") != string::npos ||
                            new_matrix[row][col].find("v") != string::npos) {
                            new_matrix[row][col] =
                                appendDirection(new_matrix[row][col], ">");
                        } else {
                            new_matrix[row][col] = ">";
                        }
                        exit_while = true;
                        break;
                    }
                }
                col--;
                if (is_loop || exit_while)
                    break;

                // down
                while (row < new_matrix.size() - 1 &&
                       new_matrix[row][col] != "#") {
                    if (new_matrix[row][col].find("v") != string::npos) {
                        is_loop = true;
                        break;
                    }
                    if (new_matrix[row][col].find(">") != string::npos ||
                        new_matrix[row][col].find("<") != string::npos) {
                        new_matrix[row][col] =
                            appendDirection(new_matrix[row][col], "v");
                    } else {
                        new_matrix[row][col] = "v";
                    }
                    row++;
                    if (new_matrix[row][col] != "#" &&
                        row == new_matrix.size() - 1) {
                        if (new_matrix[row][col].find(">") != string::npos ||
                            new_matrix[row][col].find("<") != string::npos) {
                            new_matrix[row][col] =
                                appendDirection(new_matrix[row][col], "v");
                        } else {
                            new_matrix[row][col] = "v";
                        }
                        exit_while = true;
                        break;
                    }
                }
                row--;
                if (is_loop || exit_while)
                    break;

                // left
                while (col > 0 && new_matrix[row][col] != "#") {
                    if (new_matrix[row][col].find("<") != string::npos) {
                        is_loop = true;
                        break;
                    }
                    if (new_matrix[row][col].find("∧") != string::npos ||
                        new_matrix[row][col].find("v") != string::npos) {
                        new_matrix[row][col] =
                            appendDirection(new_matrix[row][col], "<");
                    } else {
                        new_matrix[row][col] = "<";
                    }
                    col--;
                    if (new_matrix[row][col] != "#" && col == 0) {
                        if (new_matrix[row][col].find("∧") != string::npos ||
                            new_matrix[row][col].find("v") != string::npos) {
                            new_matrix[row][col] =
                                appendDirection(new_matrix[row][col], "<");
                        } else {
                            new_matrix[row][col] = "<";
                        }
                        exit_while = true;
                        break;
                    }
                }
                col++;
                if (is_loop || exit_while)
                    break;
            }

            if (is_loop == true) {
                loop++;
            }
        }
    }
    return loop;
}

int main() {
    string filename = "example.txt";
    ifstream file(filename);
    ofstream File_part1("output_part1.txt");
    ofstream File_part2("output_part2.txt");

    string line_1;

    vector<vector<string>> matrix, output_matrix;
    vector<string> line;

    while (getline(file, line_1)) {
        line.clear();
        for (size_t i = 0; i < line_1.size(); i++) {
            line.push_back(string(1, line_1[i]));
        }
        matrix.push_back(line);
    }

    vector<int> guard_location = find_guard(matrix);
    output_matrix = draw_route(matrix, guard_location);

    // output part 1
    for (size_t i = 0; i < output_matrix.size(); i++) {
        for (size_t j = 0; j < output_matrix[i].size(); j++) {
            File_part1 << output_matrix[i][j];
        }
        File_part1 << endl;
    }
    int count = count_X(output_matrix);

    output_matrix = draw_loop(matrix, guard_location);

    // output part 2
    for (size_t i = 0; i < output_matrix.size(); i++) {
        for (size_t j = 0; j < output_matrix[i].size(); j++) {
            File_part2 << output_matrix[i][j];
        }
        File_part2 << endl;
    }
    int loop = detect_loop(matrix, guard_location);

    file.close();
    File_part1.close();
    File_part2.close();

    cout << "The guard will visit " << count
         << " distinct positions before leaving the mapped area." << endl;

    cout << "You could choose " << loop
         << " different positions for the guard to get stuck." << endl;

    return 0;
}
