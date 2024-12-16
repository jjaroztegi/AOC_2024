#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <chrono>

using namespace std;
using namespace chrono;

using CacheType = unordered_map<long long, unordered_map<int, long long>>;

CacheType cache;

vector<long long> blink(long long stone)
{
    vector<long long> stones;
    if (stone == 0)
    {
        stones.push_back(1);
        return stones;
    }

    int numDigits = 0;
    long long temp = stone;
    while (temp != 0)
    {
        temp /= 10;
        numDigits++;
    }

    if (numDigits % 2 == 0)
    {
        int tenPower = 1;
        int tmp = numDigits / 2;
        for (size_t i = 0; i < tmp; i++)
        {
            tenPower *= 10;
        }

        long long firstHalf = stone / tenPower;
        stones.push_back(firstHalf);

        long long secondHalf = stone % tenPower;
        stones.push_back(secondHalf);
    }
    else
    {
        long long newNum = stone * 2024;
        stones.push_back(newNum);
    }
    return stones;
}

long long count_one_stone(long long stone, int blinks)
{
    if (blinks == 0)
    {
        return 1;
    }

    if (cache[stone].find(blinks) != cache[stone].end())
    {
        return cache[stone][blinks];
    }

    long long count = 0;
    vector<long long> stones = blink(stone);
    for (size_t i = 0; i < stones.size(); i++)
    {
        count += count_one_stone(stones[i], blinks - 1);
    }

    cache[stone][blinks] = count;
    return count;
}

long long count_all_stones(vector<int> arrangement, int blinks)
{
    long long count = 0;

    for (size_t i = 0; i < arrangement.size(); i++)
    {
        count += count_one_stone(arrangement[i], blinks);
    }

    return count;
}

int main()
{
    auto start = high_resolution_clock::now();

    string filename = "input.txt";
    ifstream input(filename);

    string line;
    vector<int> arrangement;
    getline(input, line);
    stringstream ss(line);

    int value;
    while (ss >> value)
    {
        arrangement.push_back(value);
    }
    input.close();

    int blinks = 75;
    long long count = 0;
    count += count_all_stones(arrangement, blinks);
    cout << "You will have " << count << " stones after blinking " << blinks << " times." << endl;

    auto end = high_resolution_clock::now();
    cout << "Recursive method took " << duration_cast<nanoseconds>(end - start).count() / 1000000.0 << " ms." << endl;
    return 0;
}