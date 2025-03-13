#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<int> calc_antinodes(const vector<int> &coor) {
    vector<int> result;

    int x1 = coor[0], y1 = coor[1];
    int x2 = coor[2], y2 = coor[3];

    int dx = x2 - x1;
    int dy = y2 - y1;

    if ((x2 > x1)) {
        result.push_back(x1 - dx);
        result.push_back(y1 - dy);

        result.push_back(x2 + dx);
        result.push_back(y2 + dy);
    } else if (x2 < x1) {
        result.push_back(x1 + dx);
        result.push_back(y1 - dy);

        result.push_back(x2 - dx);
        result.push_back(y2 + dy);
    }

    return result;
}

vector<vector<string>> draw_antinodes(const vector<vector<string>> &arr,
                                      const vector<int> &location,
                                      const string antenna, int &count,
                                      vector<vector<bool>> &counted) {
    vector<vector<string>> new_arr = arr;
    for (size_t i = 0; i < location.size(); i += 2) {
        int x = location[i], y = location[i + 1];
        if (x < 0 || y < 0 || x >= new_arr.size() || y >= new_arr[0].size()) {
            continue;
        }
        if (arr[x][y] == "." || new_arr[x][y] == "#") {
            new_arr[x][y] = "#";
        } else if (arr[x][y] != antenna && !counted[x][y]) {
            count++;
            counted[x][y] = true;
        }
    }
    return new_arr;
}

vector<vector<string>> create_antinodes(const vector<vector<string>> &arr,
                                        int &count) {
    vector<vector<string>> changed_arr = arr;
    vector<string> unique_antennas;

    vector<vector<bool>> counted(arr.size(),
                                 vector<bool>(arr[0].size(), false));

    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = 0; j < arr[i].size(); j++) {
            if (arr[i][j] != ".") {
                if (unique_antennas.empty()) {
                    unique_antennas.push_back(arr[i][j]);
                } else {
                    bool exists = false;
                    for (size_t k = 0; k < unique_antennas.size(); k++) {
                        if (arr[i][j] == unique_antennas[k]) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        unique_antennas.push_back(arr[i][j]);
                    }
                }
            }
        }
    }

    for (size_t k = 0; k < unique_antennas.size(); k++) {
        string antenna = unique_antennas[k];

        vector<vector<int>> antenna_coords;
        for (size_t i = 0; i < arr.size(); i++) {
            for (size_t j = 0; j < arr[i].size(); j++) {
                if (arr[i][j] == antenna) {
                    vector<int> temp_vector = {static_cast<int>(i),
                                               static_cast<int>(j)};
                    antenna_coords.push_back(temp_vector);
                }
            }
        }

        if (antenna_coords.size() < 2) {
            continue;
        }

        for (size_t i = 0; i < antenna_coords.size() - 1; i++) {
            for (size_t j = i + 1; j < antenna_coords.size(); j++) {
                vector<int> first_coord = antenna_coords[i];
                vector<int> second_coord = antenna_coords[j];

                vector<int> coordinates = first_coord;
                coordinates.push_back(second_coord[0]);
                coordinates.push_back(second_coord[1]);

                vector<int> location = calc_antinodes(coordinates);
                changed_arr = draw_antinodes(changed_arr, location, antenna,
                                             count, counted);
            }
        }
    }
    return changed_arr;
}
vector<vector<string>> create_full_antinodes(const vector<vector<string>> &arr,
                                             int &count_2) {
    vector<vector<string>> changed_arr = arr;
    vector<string> unique_antennas;

    vector<vector<bool>> counted(arr.size(),
                                 vector<bool>(arr[0].size(), false));

    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = 0; j < arr[i].size(); j++) {
            if (arr[i][j] != ".") {
                if (unique_antennas.empty()) {
                    unique_antennas.push_back(arr[i][j]);
                } else {
                    bool exists = false;
                    for (size_t k = 0; k < unique_antennas.size(); k++) {
                        if (arr[i][j] == unique_antennas[k]) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        unique_antennas.push_back(arr[i][j]);
                    }
                }
            }
        }
    }

    for (size_t k = 0; k < unique_antennas.size(); k++) {
        string antenna = unique_antennas[k];

        vector<vector<int>> antenna_coords;
        for (size_t i = 0; i < arr.size(); i++) {
            for (size_t j = 0; j < arr[i].size(); j++) {
                if (arr[i][j] == antenna) {
                    vector<int> temp_vector = {static_cast<int>(i),
                                               static_cast<int>(j)};
                    antenna_coords.push_back(temp_vector);
                }
            }
        }

        if (antenna_coords.size() < 2) {
            continue;
        }

        for (size_t i = 0; i < antenna_coords.size() - 1; i++) {
            for (size_t j = i + 1; j < antenna_coords.size(); j++) {
                vector<int> first_coord = antenna_coords[i];
                vector<int> second_coord = antenna_coords[j];

                vector<int> coordinates = first_coord;
                coordinates.push_back(second_coord[0]);
                coordinates.push_back(second_coord[1]);

                vector<int> location = calc_antinodes(coordinates);
                changed_arr = draw_antinodes(changed_arr, location, antenna,
                                             count_2, counted);

                bool out_of_bounds = false;
                int x1 = coordinates[0];
                int y1 = coordinates[1];
                int x2 = coordinates[2];
                int y2 = coordinates[3];
                int next_x1 = location[0];
                int next_y1 = location[1];
                int next_x2 = location[2];
                int next_y2 = location[3];

                while (!out_of_bounds) {

                    vector<int> next_coordinates = {next_x1, next_y1, x1, y1};
                    vector<int> next_location =
                        calc_antinodes(next_coordinates);

                    if (next_location[0] < 0 || next_location[1] < 0 ||
                        next_location[2] < 0 || next_location[3] < 0 ||
                        next_location[0] >= arr.size() ||
                        next_location[1] >= arr[0].size() ||
                        next_location[2] >= arr.size() ||
                        next_location[3] >= arr[0].size()) {
                        out_of_bounds = true;
                        break;
                    }
                    changed_arr = draw_antinodes(changed_arr, next_location,
                                                 antenna, count_2, counted);

                    x1 = next_x1;
                    y1 = next_y1;
                    next_x1 = next_location[0];
                    next_y1 = next_location[1];
                }

                out_of_bounds = false;
                while (!out_of_bounds) {

                    vector<int> next_coordinates = {x2, y2, next_x2, next_y2};
                    vector<int> next_location =
                        calc_antinodes(next_coordinates);

                    if (next_location[0] < 0 || next_location[1] < 0 ||
                        next_location[2] < 0 || next_location[3] < 0 ||
                        next_location[0] >= arr.size() ||
                        next_location[1] >= arr[0].size() ||
                        next_location[2] >= arr.size() ||
                        next_location[3] >= arr[0].size()) {
                        out_of_bounds = true;
                        break;
                    }

                    changed_arr = draw_antinodes(changed_arr, next_location,
                                                 antenna, count_2, counted);
                    x2 = next_x2;
                    y2 = next_y2;
                    next_x2 = next_location[2];
                    next_y2 = next_location[3];
                }
            }
        }
    }
    return changed_arr;
}

int count_anti(const vector<vector<string>> &antinode_arr) {
    int internal_count = 0;
    for (size_t i = 0; i < antinode_arr.size(); i++) {
        for (size_t j = 0; j < antinode_arr[i].size(); j++) {
            if (antinode_arr[i][j] == "#") {
                internal_count++;
            }
        }
    }
    return internal_count;
}

int count_full_anti(const vector<vector<string>> &antinode_arr) {
    int internal_count = 0;
    for (size_t i = 0; i < antinode_arr.size(); i++) {
        for (size_t j = 0; j < antinode_arr[i].size(); j++) {
            if (antinode_arr[i][j] != ".") {
                internal_count++;
            }
        }
    }
    return internal_count;
}

int main() {
    string filename = "example.txt";
    ifstream file(filename);
    ofstream output_file_part1("output.txt");
    ofstream output_file_part2("output_part2.txt");

    string line_1;
    vector<vector<string>> arr;

    while (getline(file, line_1)) {
        vector<string> vec;
        for (size_t i = 0; i < line_1.size(); i++) {
            vec.push_back(string(1, line_1[i]));
        }
        arr.push_back(vec);
    }

    int count = 0;
    vector<vector<string>> antinode_arr = create_antinodes(arr, count);
    count += count_anti(antinode_arr);

    for (size_t i = 0; i < antinode_arr.size(); i++) {
        for (size_t j = 0; j < antinode_arr[i].size(); j++) {
            output_file_part1 << antinode_arr[i][j];
        }
        output_file_part1 << endl;
    }

    int count_2 = 0;
    vector<vector<string>> full_antinode_arr =
        create_full_antinodes(arr, count_2);
    count_2 = count_full_anti(full_antinode_arr);

    for (size_t i = 0; i < full_antinode_arr.size(); i++) {
        for (size_t j = 0; j < full_antinode_arr[i].size(); j++) {
            output_file_part2 << full_antinode_arr[i][j];
        }
        output_file_part2 << endl;
    }

    cout << "Part 1: There are " << count
         << " unique locations within the bounds of the map that contain an "
            "antinode."
         << endl;
    cout << "Part 2: There are " << count_2
         << " unique locations within the bounds of the map that contain an "
            "antinode."
         << endl;

    return 0;
}