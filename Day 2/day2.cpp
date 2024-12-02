#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

int check_safe(const vector<int> &arr)
{
    bool increase;

    for (int i = 0; i < arr.size() - 1; i++)
    {
        int order = arr[i + 1] - arr[i];

        if (order == 0)
        {
            return 0;
        }

        if (i == 0)
        {
            increase = (order > 0);
        }
        else if ((increase && order <= 0) || (!increase && order >= 0))
        {
            return 0;
        }

        if (abs(order) > 3)
        {
            return 0;
        }
    }
    return 1;
}

int check_act_safe(vector<int> &arr)
{
    vector<int> arr_copy = arr;

    for (int i = 0; i < arr.size(); i++)
    {
        arr.erase(arr.begin() + i);

        if (check_safe(arr))
        {
            return 1;
        }

        arr = arr_copy;
    }

    return 0;
}

int main()
{
    int safe = 0;
    int act_safe = 0;
    string line;

    ifstream input("input.txt");
    while (getline(input, line))
    {
        stringstream ss(line);
        vector<int> arr;
        int value;

        while (ss >> value)
        {
            arr.push_back(value);
        }

        safe += check_safe(arr);
        act_safe += check_act_safe(arr);
    }

    cout << safe << " reports are safe." << endl;
    cout << act_safe << " reports are actually safe!" << endl;

    return 0;
}