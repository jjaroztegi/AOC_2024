#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using namespace std;
using namespace chrono;

const int width = 101;
const int height = 103;
const int num_seconds = 100;

int teleport(int value, int limit) {
    if (value < 0) {
        return value + limit;
    } else if (value >= limit) {
        return value - limit;
    }
    return value;
}

void calc_robot(vector<vector<int>> &room, int start_x, int start_y,
                const int &dx, const int &dy, int &elapsed_time,
                const int &stop_time) {
    if (elapsed_time == stop_time) {
        room[start_y][start_x]++;
        return;
    }

    int new_x = teleport(start_x + dx, width);
    int new_y = teleport(start_y + dy, height);

    elapsed_time++;

    calc_robot(room, new_x, new_y, dx, dy, elapsed_time, stop_time);
}

void calc_all_robots(const vector<vector<int>> &input_mat,
                     vector<vector<int>> &room, const int &stop_time) {
    for (size_t i = 0; i < input_mat.size(); i++) {
        int elapsed_time = 0;

        int start_x = input_mat[i][0];
        int start_y = input_mat[i][1];
        int dx = input_mat[i][2];
        int dy = input_mat[i][3];

        calc_robot(room, start_x, start_y, dx, dy, elapsed_time, stop_time);
    }
}

void find_tree(const vector<vector<int>> &input_mat,
               vector<vector<int>> &christmas_tree) {
    int stop_time = 101;
    bool found = false;

    for (stop_time = 101; stop_time < 101 * 103; ++stop_time) {
        fill(christmas_tree.begin(), christmas_tree.end(),
             vector<int>(width, 0));

        calc_all_robots(input_mat, christmas_tree, stop_time);

        bool found_contiguous_ones = false;

        for (size_t i = 0; i < height; i++) {
            int count = 0;
            for (size_t j = 0; j < width; j++) {
                if (christmas_tree[i][j] == 1) {
                    count++;
                    if (count >= 10) {
                        found_contiguous_ones = true;
                        break;
                    }
                } else {
                    count = 0;
                }
            }
            if (found_contiguous_ones) {
                break;
            }
        }

        if (!found_contiguous_ones) {
            for (size_t j = 0; j < width; j++) {
                int count = 0;
                for (size_t i = 0; i < height; i++) {
                    if (christmas_tree[i][j] == 1) {
                        count++;
                        if (count >= 10) {
                            found_contiguous_ones = true;
                            break;
                        }
                    } else {
                        count = 0;
                    }
                }
                if (found_contiguous_ones) {
                    break;
                }
            }
        }

        if (found_contiguous_ones) {
            cout << "Christmas tree found at time: " << stop_time << "\n";
            for (size_t k = 0; k < height; k++) {
                for (size_t l = 0; l < width; l++) {
                    cout << (christmas_tree[k][l] > 0 ? "*" : " ");
                }
                cout << "\n";
            }
            cout << "\n";
            break;
        }
    }
}

int main() {
    auto start = high_resolution_clock::now();

    string filename = "example.txt";
    ifstream input_file(filename);
    ofstream output_file("output.txt");

    string line;
    vector<vector<int>> input_mat;

    regex pattern("[-]?\\d+");

    while (getline(input_file, line)) {
        vector<int> tmp_vec;

        for (sregex_iterator it(line.begin(), line.end(), pattern), end_it;
             it != end_it; ++it) {
            tmp_vec.push_back(stoi(it->str()));
        }

        input_mat.push_back(tmp_vec);
    }

    vector<vector<int>> room(height, vector<int>(width));
    vector<vector<int>> christmas_tree(height, vector<int>(width));

    calc_all_robots(input_mat, room, num_seconds);
    find_tree(input_mat, christmas_tree);

    int cnt_quadrant1 = 0;
    int cnt_quadrant2 = 0;
    int cnt_quadrant3 = 0;
    int cnt_quadrant4 = 0;

    int rows = height / 2;
    int cols = width / 2;

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (i < rows && j < cols) {
                cnt_quadrant1 += room[i][j];
            } else if (i < rows && j > cols) {
                cnt_quadrant2 += room[i][j];
            } else if (i > rows && j < cols) {
                cnt_quadrant3 += room[i][j];
            } else if (i > rows && j > cols) {
                cnt_quadrant4 += room[i][j];
            }
        }
    }

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            output_file << christmas_tree[i][j];
        }
        output_file << endl;
    }

    int safety_factor =
        cnt_quadrant1 * cnt_quadrant2 * cnt_quadrant3 * cnt_quadrant4;

    cout << "The safety factor be after exactly 100 seconds have elapsed will "
            "be "
         << safety_factor << endl;

    auto end = high_resolution_clock::now();
    cout << "Day14 took "
         << duration_cast<nanoseconds>(end - start).count() / 1000000.0
         << " ms." << endl;
    return 0;
}