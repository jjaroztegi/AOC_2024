#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

bool check_order(vector<int> vec, vector<vector<int>> matrix) {
    for (size_t j = 0; j < matrix.size(); j++) {
        int num1 = -1;
        int num2 = -1;

        for (size_t i = 0; i < vec.size(); i++) {
            if (vec[i] == matrix[j][0]) {
                num1 = i;
            } else if (vec[i] == matrix[j][1]) {
                num2 = i;
            }
        }

        if (num1 > num2 && num1 != -1 && num2 != -1) {
            return false;
        }
    }
    return true;
}

vector<int> fix_order(vector<int> vec, vector<vector<int>> matrix) {
    bool swap;
    do {
        swap = false;
        for (size_t j = 0; j < matrix.size(); j++) {
            int num1 = -1;
            int num2 = -1;

            for (size_t i = 0; i < vec.size(); i++) {
                if (vec[i] == matrix[j][0]) {
                    num1 = i;
                } else if (vec[i] == matrix[j][1]) {
                    num2 = i;
                }
            }

            if (num1 > num2 && num1 != -1 && num2 != -1) {
                int tmp = vec[num1];
                vec[num1] = vec[num2];
                vec[num2] = tmp;
                swap = true;
            }
        }
    } while (swap);
    return vec;
}

int main() {
    string filename = "example.txt";
    ifstream file(filename);

    string line_1;

    size_t cols = 2;
    size_t rows = 0;

    while (getline(file, line_1)) {
        if (line_1.empty()) {
            break;
        }
        rows++;
    }

    vector<vector<int>> matrix(rows, vector<int>(cols, 0));
    vector<int> vec;

    file.clear();
    file.seekg(0, ios::beg);

    string line_2;
    bool enabled = false;
    int count_rows = 0;
    int count_right = 0;
    int count_wrong = 0;

    while (getline(file, line_2)) {
        if (line_2.empty()) {
            enabled = true;
            continue;
        } else if (enabled) {
            stringstream ss(line_2);
            while (getline(ss, line_2, ',')) {
                vec.push_back(stoi(line_2));
            }
            if (check_order(vec, matrix)) {
                count_right += vec[vec.size() / 2];
            } else {
                vec = fix_order(vec, matrix);
                count_wrong += vec[vec.size() / 2];
            }
            vec.clear();
        } else {
            stringstream ss(line_2);
            ss >> matrix[count_rows][0];
            ss.ignore(1);
            ss >> matrix[count_rows][1];
            count_rows++;
        }
    }

    cout << "Adding up the middle page number from the correctly-ordered "
            "updates gives "
         << count_right << endl;
    cout << "Adding up the middle page numbers after correctly ordering just "
            "the wrong ones gives "
         << count_wrong << endl;

    return 0;
}