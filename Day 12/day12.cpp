#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// right, down, left, up
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

struct Region
{
    int perimeter = 0;
    int area = 1;
    int price = 0;

    int sides = 0;
    int price_part2 = 0;
};

bool check_same(int i, int j, const vector<vector<string>> &input_matrix, const string &plant)
{
    int rows = input_matrix.size();
    int cols = input_matrix[0].size();
    return i >= 0 && i < rows && j >= 0 && j < cols && input_matrix[i][j] == plant;
}

int get_corners(int i, int j, const vector<vector<string>> &input_matrix, vector<vector<bool>> &visited)
{
    int corners = 0;
    int rows = input_matrix.size();
    int cols = input_matrix[0].size();
    string plant = input_matrix[i][j];

    // Each N, S, E, W, NW, NE, SW, SE checks whether the plant at the current position has the same plant
    // adjacent 8 positions (NW, NE, SW, SE are the diagonals).
    bool N = check_same(i - 1, j, input_matrix, plant);
    bool S = check_same(i + 1, j, input_matrix, plant);
    bool E = check_same(i, j + 1, input_matrix, plant);
    bool W = check_same(i, j - 1, input_matrix, plant);

    bool NW = check_same(i - 1, j - 1, input_matrix, plant);
    bool NE = check_same(i - 1, j + 1, input_matrix, plant);
    bool SW = check_same(i + 1, j - 1, input_matrix, plant);
    bool SE = check_same(i + 1, j + 1, input_matrix, plant);

    // +1 corner if there is a "hole"
    corners = N && W && !NW;
    corners += N && E && !NE;
    corners += S && W && !SW;
    corners += S && E && !SE;
    // +1 corner no adjacent plants in "diagonal" direction
    // this only works because plant grow just horizontally or vertically
    corners += !(N || W);
    corners += !(N || E);
    corners += !(S || W);
    corners += !(S || E);

    return corners;
}

void get_region(const vector<vector<string>> &input_matrix, vector<vector<bool>> &visited, int start_x, int start_y, Region &region)
{
    visited[start_x][start_y] = true;

    region.sides += get_corners(start_x, start_y, input_matrix, visited);

    int rows = input_matrix.size();
    int cols = input_matrix[0].size();
    for (int i = 0; i < 4; i++)
    {
        int new_x = start_x + dx[i];
        int new_y = start_y + dy[i];

        if (new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols)
        {
            region.perimeter++;
        }
        else if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols &&
                 !visited[new_x][new_y] && input_matrix[new_x][new_y] == input_matrix[start_x][start_y])
        {
            region.area++;
            get_region(input_matrix, visited, new_x, new_y, region);
        }
        else if (input_matrix[new_x][new_y] != input_matrix[start_x][start_y])
        {
            region.perimeter++;
        }
    }
}

vector<int> get_all_regions(const vector<vector<string>> &input_matrix)
{
    int rows = input_matrix.size();
    int cols = input_matrix[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    int total_price = 0;
    int total_price_part2 = 0;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (!visited[i][j])
            {
                Region region;
                get_region(input_matrix, visited, i, j, region);
                region.price = region.area * region.perimeter;
                total_price += region.price;

                region.price_part2 = region.area * region.sides;
                total_price_part2 += region.price_part2;
            }
        }
    }
    return {total_price, total_price_part2};
}

int main()
{
    string filename = "input.txt";
    ifstream input_file(filename);

    string line;

    vector<vector<string>> input_matrix;
    while (getline(input_file, line))
    {
        vector<string> row;
        for (size_t i = 0; i < line.size(); i++)
        {
            row.push_back(string(1, line[i]));
        }
        input_matrix.push_back(row);
    }
    input_file.close();

    vector<int> result = get_all_regions(input_matrix);

    cout << "Total price of fencing all regions is " << result[0] << endl;
    cout << "New total price of fencing all regions is " << result[1];

    return 0;
}