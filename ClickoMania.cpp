//
// Created by stefa on 4/28/2020.
//
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

/*
20 10 2
BBRBRBRBBB
RBRBRBBRRR
RRRBBRBRRR
RBRBRRRBBB
RBRBRRRRBB
RBBRBRRRRR
BBRBRRBRBR
BRBRBBRBBB
RBBRRRRRRB
BBRBRRBBRB
BBBRBRRRBB
BRBRRBRRBB
BRRBBBBBRB
RRBBRRBRRR
RRRBRRRBBB
RRRRRBBBRR
BRRRBRRRBB
BBBBRBRRRB
BRBBBBBRBB
RRRRRBBRRR
*/

struct Point {
    int row;
    int col;
    Point() {}
    Point(int r, int c) { row = r; col = c; }
};

struct Game {
    Game(){}
    void print_grid(vector<string> grid) {
        for(int i=0; i<grid.size(); i++) {
            cout << grid[i] << endl;
        }
    }
    void print_cell(Point p) {
        cout << p.row << " " << p.col << endl;
    }
    vector<char> get_distinct_colors(vector<string> &grid) {
        vector<char> colors;
        for(int i=0; i<grid.size(); i++) {
            for(int j=0; j<grid[i].length(); j++) {
                if (grid[i][j] != '-') {
                    if (find(colors.begin(), colors.end(), grid[i][j]) == colors.end()) {
                        colors.push_back(grid[i][j]);
                    }
                }
            }
        }
        return colors;
    }
    int get_adjacent_cells(vector<string> &grid, int row, int col, char color, bool init) {
        int result = 0;
        static vector<string> grid_copy;
        if (color == '-') {
            return 0;
        }
        if (init == true) {
            grid_copy = grid;
        }
        if (grid[row][col] == color) {
            result++;
            grid_copy[row][col] = '-';
            if (row - 1 >= 0 && grid[row - 1][col] == color) {
                result += get_adjacent_cells(grid_copy, row - 1, col, color, false);
            }
            if (row + 1 < grid.size() && grid[row + 1][col] == color) {
                result += get_adjacent_cells(grid_copy, row + 1, col, color, false);
            }
            if (col - 1 >= 0 && grid[row][col-1] == color) {
                result += get_adjacent_cells(grid_copy, row, col - 1, color, false);
            }
            if (col + 1 < grid[row].length() && grid[row][col+1] == color) {
                result += get_adjacent_cells(grid_copy, row, col + 1, color, false);
            }
        }
        return result;
    }
    Point next_move(int rows, int cols, int num_colors, vector<string> &grid) {
        vector<char> colors = get_distinct_colors(grid);
        multimap<int, Point, greater<int>> maps;
        for(int r=0; r<grid.size(); r++) {
            for(int c=0; c<grid[r].length(); c++) {
                int cells = get_adjacent_cells(grid, r, c, grid[r][c], true);
                Point point(r, c);
                maps.insert(make_pair(cells, point));
            }
        }
        return maps.begin()->second;
    }
    void delete_adjacent_cells(vector<string> &grid, int row, int col, char color) {
        if (grid[row][col] == color) {
            grid[row][col] = '-';
            if (row - 1 >= 0 && grid[row - 1][col] == color) {
                delete_adjacent_cells(grid, row - 1, col, color);
            }
            if (row + 1 < grid.size() && grid[row + 1][col] == color) {
                delete_adjacent_cells(grid, row + 1, col, color);
            }
            if (col - 1 >= 0 && grid[row][col - 1] == color) {
                delete_adjacent_cells(grid, row, col - 1, color);
            }
            if (col + 1 < grid[row].length() && grid[row][col + 1] == color) {
                delete_adjacent_cells(grid, row, col + 1, color);
            }
        }
    }
    void down_falling_cells(vector<string> &grid) {
        vector<string> new_grid;
        for(int r=0; r<grid.size(); r++) {
            string line;
            for(int c=0; c<grid[r].length(); c++) {
                line.insert(line.end(), '-');
            }
            new_grid.push_back(line);
        }
        for(int c=0; c<grid[0].length(); c++) {
            int new_row = grid.size()-1;
            for(int r=grid.size()-1; r>=0; r--) {
                if (grid[r][c] != '-') {
                    new_grid[new_row--][c] = grid[r][c];
                }
            }
        }
        grid = new_grid;
        new_grid.clear();
        for(int r=0; r<grid.size(); r++) {
            bool row_empty = true;
            for(int c=0; c<grid[r].length(); c++) {
                if (grid[r][c] != '-') {
                    row_empty = false;
                }
            }
            if (row_empty == false) {
                new_grid.push_back(grid[r]);
            }
        }
        grid = new_grid;
    }
    void left_falling_cells(vector<string> &grid) {
        for(int c=0; c<grid[0].length(); c++) {
            bool column_empty = true;
            for(int r=0; r<grid.size(); r++) {
                if (grid[r][c] != '-') {
                    column_empty = false;
                }
            }
            if (column_empty == true) {
                for(int r=0; r<grid.size(); r++) {
                    grid[r].erase(c, 1);
                }
                c--;
            }
        }
    }
    int get_cells(vector<string> &grid) {
        int result = 0;
        for(int r=0; r<grid.size(); r++) {
            for(int c=0; c<grid[r].length(); c++) {
                if (grid[r][c] != '-') {
                    result++;
                }
            }
        }
        return result;
    }
    void play_game(int rows, int cols, int num_colors, vector<string> &grid) {
        int round = 0;
        while(get_cells(grid) >= 1) {
            cout << "####### ROUND " << ++round << " #######" << endl;
            print_grid(grid);
            Point cell = next_move(rows, cols, num_colors, grid);
            print_cell(cell);
            delete_adjacent_cells(grid, cell.row, cell.col, grid[cell.row][cell.col]);
            down_falling_cells(grid);
            left_falling_cells(grid);
        }
    }
};

void nextMove(int x, int y, int color, vector <string> grid){
    //your logic here
    Game game;
    vector<char> colors = game.get_distinct_colors(grid);
    multimap<int, Point, greater<int>> maps;
    for(int r=0; r<grid.size(); r++) {
         for(int c=0; c<grid[r].length(); c++) {
            int cells = game.get_adjacent_cells(grid, r, c, grid[r][c], true);
            Point point(r, c);
            maps.insert(make_pair(cells, point));
            //cout << r << ", " << c << ", color: " << grid[r][c] << ", cells: " << cells << endl;
        }
    }
    cout << "max cells: " << maps.begin()->first << ", point: "
        << maps.begin()->second.row << ", " << maps.begin()->second.col << endl;
    //cout << maps.begin()->second.row << " " << maps.begin()->second.col << endl;


    int row = maps.begin()->second.row;
    int col = maps.begin()->second.col;
    game.delete_adjacent_cells(grid, row, col, grid[row][col]);
    for(int r=0; r<grid.size(); r++) {
        cout << grid[r] << endl;
    }
}

int main(void) {

    int x,y,k;
    cin >> x >> y >> k;
    vector <string> grid;

    for(int i=0; i<x; i++) {
        string s; cin >> s;
        grid.push_back(s);
    }

    //nextMove( x, y, k, grid);

    Game game;
    game.play_game(x, y, k, grid);

    return 0;
}
