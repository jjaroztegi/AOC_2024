#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

long long calculate(long long num1, long long num2, int operation) {
    if (operation == 0) {
        return num1 + num2;
    } else if (operation == 1) {
        return num1 * num2;
    } else {
        string str_num1 = to_string(num1);
        string str_num2 = to_string(num2);

        return stoll(str_num1 + str_num2);
    }
}

bool solve_equation(const vector<long long> &arr, int index,
                    long long current_result, long long correct_result,
                    vector<int> &operation_history) {
    if (index == arr.size()) {
        if (current_result == correct_result) {
            return true;
        }
        return false;
    }

    for (int operation : {0, 1, 2}) {
        long long new_result = calculate(current_result, arr[index], operation);
        operation_history.push_back(operation);

        // Recurse
        if (solve_equation(arr, index + 1, new_result, correct_result,
                           operation_history)) {
            return true;
        }
        // Backtrack
        operation_history.pop_back();
    }

    return false;
}

int main() {
    string filename = "example.txt";
    ifstream file(filename);

    long long result = 0;
    string line_1;

    while (getline(file, line_1)) {
        stringstream ss(line_1);
        vector<long long> arr;
        long long value;

        // First number
        ss >> value;
        arr.push_back(value);
        ss.ignore(1);

        // Rest
        while (ss >> value) {
            arr.push_back(value);
        }

        long long correct_result = arr[0];
        vector<int> operation_history;

        if (solve_equation(arr, 2, arr[1], correct_result, operation_history)) {
            result += correct_result;
        }
    }

    file.close();
    cout << "The total calibration result is " << result << endl;

    return 0;
}