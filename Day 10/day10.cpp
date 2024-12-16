#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

bool isCorrect(const vector<vector<int>> &matrix, int x, int y, int new_x, int new_y)
{
    int rows = matrix.size();
    int cols = matrix[0].size();
    if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols && matrix[new_x][new_y] == matrix[x][y] + 1)
    {
        return true;
    }
    return false;
}

void calc_trail(const vector<vector<int>> &matrix, int start_x, int start_y, vector<vector<int>> &current_path, vector<vector<int>> &trail_history, vector<vector<vector<int>>> &all_paths, int &count)
{
    current_path.push_back({start_x, start_y});

    if (matrix[start_x][start_y] == 9)
    {
        all_paths.push_back(current_path);
        trail_history.insert(trail_history.end(), current_path.begin(), current_path.end());
        count++;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            int new_x = start_x + dx[i];
            int new_y = start_y + dy[i];

            bool is_visited = false;
            if (trail_history.size() > 1)
            {
                for (size_t j = 0; j < trail_history.size(); j++)
                {
                    if (trail_history[j][0] == new_x && trail_history[j][1] == new_y)
                    {
                        is_visited = true;
                        break;
                    }
                }
            }
            if (!is_visited && isCorrect(matrix, start_x, start_y, new_x, new_y))
            {
                calc_trail(matrix, new_x, new_y, current_path, trail_history, all_paths, count);
            }
        }
    }
    current_path.pop_back();
}
void calc_rating(const vector<vector<int>> &matrix, int start_x, int start_y, vector<vector<int>> &current_path, vector<vector<int>> &trail_history, vector<vector<vector<int>>> &all_paths, int &rating)
{
    current_path.push_back({start_x, start_y});

    if (matrix[start_x][start_y] == 9)
    {
        all_paths.push_back(current_path);
        trail_history.insert(trail_history.end(), current_path.begin(), current_path.end());
        rating++;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            int new_x = start_x + dx[i];
            int new_y = start_y + dy[i];

            current_path.push_back({new_x, new_y});

            bool is_visited = false;
            if (trail_history.size() > 1)
            {
                bool match = true;
                for (size_t j = 0; j <= trail_history.size() - current_path.size(); j++)
                {
                    for (size_t k = 0; k < current_path.size(); k++)
                    {
                        if (trail_history[j + k] != current_path[k])
                        {
                            match = false;
                            break;
                        }
                    }
                }
                if (match)
                {
                    is_visited = true;
                    break;
                }
            }
            current_path.pop_back();

            if (!is_visited && isCorrect(matrix, start_x, start_y, new_x, new_y))
            {
                calc_rating(matrix, new_x, new_y, current_path, trail_history, all_paths, rating);
            }
        }
    }
    current_path.pop_back();
}

int calc_all_trails(const vector<vector<int>> &matrix, int start_x, int start_y, int &rating)
{
    vector<vector<int>> trail_history;
    vector<vector<int>> current_path;
    vector<vector<vector<int>>> all_paths;

    int count = 0;

    calc_trail(matrix, start_x, start_y, current_path, trail_history, all_paths, count);

    vector<vector<int>> trail_history_part2;

    calc_rating(matrix, start_x, start_y, current_path, trail_history_part2, all_paths, rating);

    return count;
}

int main()
{
    string filename = "input.txt";

    string line_1;
    ifstream input(filename);

    size_t rows = 0, cols = 0;
    while (getline(input, line_1))
    {
        if (rows == 0)
        {
            rows = line_1.size();
        }
        cols++;
    }

    vector<vector<int>> matrix(rows, vector<int>(cols, 0));

    input.clear();
    input.seekg(0, ios::beg);

    string line_2;
    int i = 0;

    vector<vector<int>> start_positions;

    while (getline(input, line_2))
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = line_2[j] - '0';
            if (matrix[i][j] == 0)
            {
                start_positions.push_back({i, j});
            }
        }
        i++;
    }

    int total_score = 0;
    int rating = 0;
    vector<int> scores;

    for (size_t i = 0; i < start_positions.size(); i++)
    {

        int start_x = start_positions[i][0];
        int start_y = start_positions[i][1];
        int score = calc_all_trails(matrix, start_x, start_y, rating);
        scores.push_back(score);
        total_score += score;
    }

    cout << "Trailhead Scores: ";
    for (int score : scores)
    {
        cout << score << " ";
    }
    cout << endl;

    cout << "Total score of all trailheads is: " << total_score << endl;
    cout << "Total score of all trailheads is: " << rating << endl;

    return 0;
}