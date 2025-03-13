#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Result {
    vector<string> str;
    vector<int> num;
};

bool isDigit(char c) { return c >= '0' && c <= '9'; }

int checkNum(string num, int start_location) {
    int size = 0;
    while (size < 3 && start_location + size < num.length()) {
        if (!isDigit(num[start_location + size])) {
            return size;
        }
        size++;
    }
    return size;
}

Result findPattern(string pattern, bool &enabled) {
    Result result;
    int location = 0;

    while (location < pattern.length() - 8) {
        if (pattern.substr(location, 4) == "mul(") {
            location += 4;
            int size1 = checkNum(pattern, location);
            string num1 = pattern.substr(location, size1);
            location += size1;

            if (size1 > 0) {
                if (pattern[location] == ',') {
                    location++;
                }
                int size2 = checkNum(pattern, location);
                string num2 = pattern.substr(location, size2);
                location += size2;

                if (size2 > 0) {
                    if (pattern[location] == ')') {
                        location++;

                        int totalSize = size1 + size2 + 1 + 1 + 4;
                        if (enabled) {
                            result.num.push_back(stoi(num1));
                            result.num.push_back(stoi(num2));
                            result.str.push_back(pattern.substr(
                                location - totalSize, totalSize));
                        }
                    }
                }
            }
        } else if (pattern.substr(location, 7) == "don't()") {
            enabled = false;
            location += 7;
        } else if (pattern.substr(location, 4) == "do()") {
            enabled = true;
            location += 4;
        } else {
            location++;
        }
    }
    return result;
}

int main() {
    ifstream file("example.txt");
    ofstream newFile("output.txt");

    string line;
    int count = 0;
    bool enabled = true;

    while (getline(file, line)) {
        Result res = findPattern(line, enabled);

        for (const auto &str : res.str) {
            newFile << str << endl;
        }

        for (int i = 0; i < res.num.size(); i += 2) {
            count += res.num[i] * res.num[i + 1];
        }
    }

    file.close();
    newFile.close();

    cout << "Sum of the results of the multiplications: " << count << endl;

    return 0;
}