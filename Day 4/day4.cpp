#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

bool checkXmas(string word) {
    if (word == "XMAS" || word == "SAMX") {
        return true;
    }
    return false;
}

bool checkMas(string word) {
    if (word == "MAS" || word == "SAM") {
        return true;
    }
    return false;
}

int findXmas(vector<vector<string>> matrix) {
    int num = 0;

    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            // rows
            if (j + 3 < matrix[i].size()) {
                string row_sub = matrix[i][j] + matrix[i][j + 1] +
                                 matrix[i][j + 2] + matrix[i][j + 3];
                num += checkXmas(row_sub);
            }
            // columns
            if (i + 3 < matrix.size()) {
                string col_sub = matrix[i][j] + matrix[i + 1][j] +
                                 matrix[i + 2][j] + matrix[i + 3][j];
                num += checkXmas(col_sub);
            }
            // diagonals
            if (i + 3 < matrix.size() && j + 3 < matrix[i].size()) {
                string diag_sub = matrix[i][j] + matrix[i + 1][j + 1] +
                                  matrix[i + 2][j + 2] + matrix[i + 3][j + 3];
                num += checkXmas(diag_sub);
            }
            // reverse diagonals
            if (i + 3 < matrix.size() && j >= 3) {
                string rev_diag_sub = matrix[i][j] + matrix[i + 1][j - 1] +
                                      matrix[i + 2][j - 2] +
                                      matrix[i + 3][j - 3];
                num += checkXmas(rev_diag_sub);
            }
        }
    }
    return num;
}

int findMas(vector<vector<string>> matrix) {
    int num = 0;

    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (i + 2 < matrix.size() && j + 2 < matrix[i].size()) {
                string diag_1 =
                    matrix[i][j] + matrix[i + 1][j + 1] + matrix[i + 2][j + 2];
                string diag_2 =
                    matrix[i + 2][j] + matrix[i + 1][j + 1] + matrix[i][j + 2];
                if (checkMas(diag_1) && checkMas(diag_2)) {
                    num++;
                }
            }
        }
    }

    return num;
}

int main() {
    string filename = "example.txt";
    ifstream file(filename);

    string line_1;

    size_t cols = 0;
    size_t rows = 0;

    while (getline(file, line_1)) {
        if (rows == 0) {
            cols = line_1.length();
        }
        rows++;
    }

    vector<vector<string>> matrix(rows, vector<string>(cols, ""));

    file.clear();
    file.seekg(0, ios::beg);

    string line_2;
    size_t i = 0;

    while (getline(file, line_2)) {
        for (size_t j = 0; j < cols; j++) {
            matrix[i][j] = line_2[j];
        }
        i++;
    }

    file.close();

    int count_xmas = findXmas(matrix);
    int count_mas = findMas(matrix);

    cout << "XMAS appears " << count_xmas << " times." << endl;
    cout << "X-MAS appears " << count_mas << " times." << endl;

    return 0;
}
