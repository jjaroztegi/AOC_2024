#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

bool is_int(const long long &num_1, const long long &num_2, const long long &divisor)
{
    if (num_1 % divisor != 0 || num_2 % divisor != 0)
    {
        return false;
    }
    return true;
}

vector<long long> solve_xy(int a1, int a2, int b1, int b2, long long c1, long long c2)
{
    long long divisor = a1 * b2 - a2 * b1;

    if (divisor == 0)
    {
        return {0, 0};
    }

    long long x_numerator = c1 * b2 - c2 * b1;
    long long y_numerator = a1 * c2 - a2 * c1;

    if (!is_int(x_numerator, y_numerator, divisor))
    {
        return {0, 0};
    }

    long long x = x_numerator / divisor;
    long long y = y_numerator / divisor;

    // enable the other two ORs for part 1
    if (x < 0 || y < 0 /*|| x >= 100 || y >= 100*/)
    {
        return {0, 0};
    }

    return {x, y};
}

long long calc_tokens(const vector<vector<int>> &input_mat)
{
    long long tokens = 0;

    for (size_t i = 0; i < input_mat.size(); i += 3)
    {
        int a1 = input_mat[i][0], a2 = input_mat[i][1];
        int b1 = input_mat[i + 1][0], b2 = input_mat[i + 1][1];
        // remove + 10000000000000 for part 1
        long long c1 = input_mat[i + 2][0] + 10000000000000, c2 = input_mat[i + 2][1] + 10000000000000;

        vector<long long> button_pushes = solve_xy(a1, a2, b1, b2, c1, c2);
        tokens += button_pushes[0] * 3 + button_pushes[1] * 1;
    }

    return tokens;
}

int main()
{
    string filename = "input.txt";
    ifstream input_file(filename);

    string line;
    vector<vector<int>> input_mat;

    regex pattern("\\d+");

    while (getline(input_file, line))
    {
        if (!line.empty())
        {
            vector<int> tmp_vec;

            for (sregex_iterator it(line.begin(), line.end(), pattern), end_it; it != end_it; ++it)
            {
                tmp_vec.push_back(stoi(it->str()));
            }

            input_mat.push_back(tmp_vec);
        }
    }

    long long total_tokens = calc_tokens(input_mat);

    cout << total_tokens << " is the fewest tokens you would have to spend to win all possible prizes." << endl;

    return 0;
}