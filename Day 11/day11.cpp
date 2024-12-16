#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>

using namespace std;
using namespace chrono;

void print_values(const vector<long long> &arrangement, int num_blinks, ofstream &output)
{
    if (num_blinks == 0)
    {
        output << "Initial arrangement:" << endl;
    }
    else if (num_blinks == 1)
    {
        output << "After " << num_blinks << " blink:" << endl;
    }
    else
    {
        output << "After " << num_blinks << " blinks:" << endl;
    }

    for (size_t i = 0; i < arrangement.size(); i++)
    {
        output << arrangement[i];
        if (i != arrangement.size() - 1)
        {
            output << " ";
        }
    }
    output << endl;
}

vector<long long> rearrange_stones(const vector<long long> &arrangement)
{
    vector<long long> new_arrangement;

    for (size_t i = 0; i < arrangement.size(); i++)
    {
        if (arrangement[i] == 0)
        {
            new_arrangement.push_back(1);
            continue;
        }

        int numDigits = 0;
        long long temp = arrangement[i];
        while (temp != 0)
        {
            temp /= 10;
            numDigits++;
        }

        if (numDigits % 2 == 0)
        {
            long long tenPower = pow(10, numDigits / 2);

            long long firstHalf = arrangement[i] / tenPower;
            new_arrangement.push_back(firstHalf);

            long long secondHalf = arrangement[i] % tenPower;
            new_arrangement.push_back(secondHalf);
        }
        else
        {
            long long newNum = arrangement[i] * 2024;
            new_arrangement.push_back(newNum);
        }
    }

    return new_arrangement;
}

int main()
{
    auto start = high_resolution_clock::now();

    string filename = "input.txt";
    ifstream input(filename);
    ofstream output("output.txt");

    string line;
    vector<long long> arrangement;
    getline(input, line);
    stringstream ss(line);
    int value;

    while (ss >> value)
    {
        arrangement.push_back(value);
    }

    input.close();

    for (size_t i = 0; i < 26; i++)
    {
        // auto start = high_resolution_clock::now();

        print_values(arrangement, i, output);

        if (i < 25)
        {
            arrangement = rearrange_stones(arrangement);
        }

        // auto end = high_resolution_clock::now();
        // cout << "Step " << i << " took " << duration_cast<milliseconds>(end - start).count() << " ms." << endl;
    }
    output.close();

    cout << "You will have " << arrangement.size() << " stones after blinking 25 times." << endl;

    auto end = high_resolution_clock::now();
    cout << "Manual method took " << duration_cast<nanoseconds>(end - start).count() / 1000000.0 << " ms." << endl;
    return 0;
}