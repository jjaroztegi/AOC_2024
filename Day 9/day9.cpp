#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

vector<string> map_to_block(const vector<int> &input_vec)
{
    vector<string> block_vec;
    size_t id = 0;

    for (size_t i = 0; i < input_vec.size(); i++)
    {
        if (i % 2 == 0)
        {
            for (size_t j = 0; j < input_vec[i]; j++)
            {
                block_vec.push_back(to_string(id));
            }
            id++;
        }
        else
        {
            for (size_t j = 0; j < input_vec[i]; j++)
            {
                block_vec.push_back(".");
            }
        }
    }

    return block_vec;
}

vector<string> remove_gaps(const vector<string> &blocks)
{
    vector<string> compact_blocks = blocks;
    for (size_t i = compact_blocks.size() - 1; i > 0; i--)
    {
        if (compact_blocks[i] == ".")
        {
            continue;
        }
        else
        {
            for (size_t j = 0; j < i; j++)
            {
                if (compact_blocks[j] == ".")
                {
                    compact_blocks[j] = compact_blocks[i];
                    compact_blocks[i] = ".";
                    break;
                }
            }
        }
    }

    return compact_blocks;
}
vector<string> remove_gaps_part2(const vector<string> &blocks)
{
    vector<string> compact_blocks = blocks;
    for (int i = compact_blocks.size() - 1; i > 0; i--)
    {
        if (compact_blocks[i] == ".")
        {
            continue;
        }

        int cnt = 1;

        while (i > 0 && compact_blocks[i] == compact_blocks[i - 1])
        {
            cnt++;
            i--;
        }

        for (int j = 0; j < i - cnt + 1; j++)
        {
            bool all_empty = true;

            for (int k = j; k < j + cnt; k++)
            {
                if (compact_blocks[k] != ".")
                {
                    all_empty = false;
                    break;
                }
            }

            if (all_empty)
            {
                for (int k = j; k < j + cnt; k++)
                {
                    compact_blocks[k] = compact_blocks[i];
                }

                for (int k = 0; k < cnt; k++)
                {
                    if (i + k < compact_blocks.size())
                    {
                        compact_blocks[i + k] = ".";
                    }
                }
                break;
            }
        }
    }

    return compact_blocks;
}

long long calc_checksum(vector<string> compact_blocks)
{
    long long count = 0;
    for (size_t i = 0; i < compact_blocks.size(); i++)
    {
        if (compact_blocks[i] == ".")
        {
            continue;
        }
        else
        {
            count += i * stoi(compact_blocks[i]);
        }
    }
    return count;
}

int main()
{
    string filename = "input.txt";
    ifstream input_file(filename);
    ofstream output_file("output.txt");
    ofstream output_file_2("output_part2.txt");

    string line;
    getline(input_file, line);

    vector<int> input_vec;
    for (size_t i = 0; i < line.size(); i++)
    {
        input_vec.push_back(line[i] - '0');
    }

    vector<string> blocks = map_to_block(input_vec);
    vector<string> compact_blocks = remove_gaps(blocks);

    for (size_t i = 0; i < compact_blocks.size(); i++)
    {
        output_file << compact_blocks[i];
    }

    vector<string> compact_blocks_part2 = remove_gaps_part2(blocks);
    for (size_t i = 0; i < compact_blocks_part2.size(); i++)
    {
        output_file_2 << compact_blocks_part2[i];
    }

    long long checksum = calc_checksum(compact_blocks);
    long long checksum_2 = calc_checksum(compact_blocks_part2);

    cout << "Part 1: The resulting filesystem checksum is: " << checksum << endl;
    cout << "Part 2: The resulting filesystem checksum is: " << checksum_2 << endl;

    return 0;
}