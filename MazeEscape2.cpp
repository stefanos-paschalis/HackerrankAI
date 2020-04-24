//
// Created by stefa on 4/15/2020.
//
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

enum Move { NONE=-1, UP, LEFT, DOWN, RIGHT };
typedef Move Orientation;

struct Point {
    int id;
    int row;
    int col;
    Orientation orientation;
    Point() {}
    Point(int r, int c) { row = r; col = c; }
};

struct Board {
    int top;
    int left;
    vector<string> cells;
};

struct Data {
    Point bot;
    Board board;
    vector<string> visited;
};

struct Game {
    Game() {}
    const string filename1 = "MazeEscape1.txt";
    const string filename2 = "MazeEscape2.txt";
    const string orientation[4] = { "UP", "LEFT", "DOWN", "RIGHT" };
    const string move[4] = { "UP", "LEFT", "DOWN", "RIGHT" };
    void init_visited(Data &data, vector<string> maze) {
        data.visited.clear();
        for (int i = 0; i < maze.size(); i++) {
            string line;
            for (int j=0; j<maze[i].length(); j++) {
                line.insert(line.end(), maze[i][j]);
            }
            data.visited.push_back(line);
        }
    }
    void read_input(Data &data) {
        cin >> data.bot.id;
        for(int i=0; i<3; i++) {
            string str;
            cin >> str;
            data.board.cells.push_back(str);
        }
    }
    void read_file(Data &data, vector<string> maze) {
        try {
            string str = "";
            string fname = "";
            if (data.bot.id == 1) {
                fname = filename1;
            } else {
                fname = filename2;
            }
            ifstream fin(fname);
            if (!fin.fail()) {
                int row, col, orientation;
                fin >> row >> col >> orientation;
                data.bot.orientation = (Orientation) orientation;
                getline(fin, str);
                data.visited.clear();
                while(getline(fin, str)) {
                    data.visited.push_back(str);
                }
                fin.close();
                if (data.bot.row < row-1 || data.bot.row > row+1
                        || data.bot.col < col-1 || data.bot.col > col+1 ) {
                    data.bot.row = 1;
                    data.bot.col = 5;
                    data.bot.orientation = Orientation::NONE;
                    init_visited(data, maze);
                    data.visited[1][5] = 'x';
                }
                data.bot.row = row;
                data.bot.col = col;
            } else {
                data.bot.row = 1;
                data.bot.col = 5;
                data.bot.orientation = Orientation::NONE;
                init_visited(data, maze);
                data.visited[1][5] = 'x';
            }
        } catch(exception e) {
            cout << e.what() << endl;
        }
    }
    void write_file(Data &data, vector<string> &maze) {
        try {
            string fname = "";
            if (data.bot.id == 1) {
                fname = filename1;
            } else {
                fname = filename2;
            }
            ofstream fout(fname/*, ofstream::trunc*/);
            fout << data.bot.row << " " << data.bot.col << " " << data.bot.orientation << endl;
            for (int i = 0; i < data.visited.size(); i++) {
                fout << data.visited[i] << endl;
            }
            fout.close();
        } catch(exception e) {
            cout << e.what() << endl;
        }
    }
    Point get_exit(vector<string> &maze) {
        int row = -1, col = -1;
        for (int i = 0; i < maze.size(); i++) {
            for (int j = 0; j < maze[i].length(); j++) {
                if (maze[i][j] == 'e') {
                    row = i;
                    col = j;
                }
            }
        }
        return Point(row, col);
    }
    vector<string> get_adjacent_cells(Data &data, vector<string> maze) {
        vector<string> adjacent_cells;
        for(int i=data.bot.row-1; i<=data.bot.row+1; i++) {
            string line = "";
            for(int j=data.bot.col-1; j<=data.bot.col+1; j++) {
                line.insert(line.end(), maze[i][j]);
            }
            adjacent_cells.push_back(line);
        }
        return adjacent_cells;
    }
    void rotate_clockwise(vector<string> &cells) {
        vector<string> rotated;
        for(int j=0; j<cells[0].length(); j++) {
            string row = "";
            for (int i = cells.size()-1; i >= 0; i--) {
                row.insert(row.end(), cells[i][j]);
            }
            rotated.push_back(row);
        }
        cells.empty();
        cells = rotated;
    }
    void rotate_counter_clockwise(vector<string> &cells) {
        vector<string> rotated;
        for(int j=cells[0].length()-1; j>=0; j--) {
            string row = "";
            for(int i=0; i<cells.size(); i++) {
                row.insert(row.end(), cells[i][j]);
            }
            rotated.push_back(row);
        }
        cells.empty();
        cells = rotated;
    }
    void get_orientation(Data &data, vector<string> &maze) {
        int rotations = 0;
        vector<string> adjacent_cells = get_adjacent_cells(data, maze);
        while(rotations < 4) {
            if (adjacent_cells == data.board.cells) {
                data.bot.orientation = (Orientation)rotations;
            }
            rotate_clockwise(adjacent_cells);
            rotations++;
        }
    }
    pair<pair<Move, Point>, Point> get_move(Data &data, vector<string> &maze) {
        pair<Move, Orientation> next_move;
        Point exit, cell, target;
        bool exit_found = false, cell_found = false, target_found = false;
        for(int i=data.bot.row-1; i<=data.bot.row+1; i++) {
            for(int j=data.bot.col-1; j<data.bot.col+1; j++) {
                if (data.visited[i][j] == 'e' && (data.bot.row-i==0 || data.bot.col-j==0)) {
                    exit.row = i;
                    exit.col = j;
                    exit_found = true;
                } else if (data.visited[i][j] == '-' && (data.bot.row-i==0 || data.bot.col-j==0)) {
                    cell.row = i;
                    cell.col = j;
                    cell_found = true;
                }
            }
        }
        if (exit_found == true) {
            target = exit;
            target_found = true;
        } else if (cell_found == true) {
            target = cell;
            target_found = true;
        }
        if (target_found == true) {
            next_move = get_next_move(data, target);
        }
        return make_pair(next_move, target);
    }
    pair<Move, Orientation> get_next_move(Data &data, Point target) {
        Move move = Move::NONE;
        Orientation orientation = Orientation::NONE;
        if (data.bot.orientation == Orientation::RIGHT) {
            if (data.bot.row + 1 == target.row) {
                move = Move::RIGHT;
                orientation = Orientation::DOWN;
            } else if (data.bot.row - 1 == target.row) {
                move = Move::LEFT;
                orientation = Orientation::UP;
            } else if (data.bot.col + 1 == target.col) {
                move = Move::UP;
                orientation = Orientation::RIGHT;
            } else if (data.bot.col - 1 == target.col) {
                move = Move::DOWN;
                orientation = Orientation::LEFT;
            }
        } else if (data.bot.orientation == Orientation::DOWN) {
            if (data.bot.row + 1 == target.row) {
                move = Move::UP;
                orientation = Orientation::DOWN;
            } else if (data.bot.row - 1 == target.row) {
                move = Move::DOWN;
                orientation = Orientation::UP;
            } else if (data.bot.col + 1 == target.col) {
                move = Move::LEFT;
                orientation = Orientation::RIGHT;
            } else if (data.bot.col - 1 == target.col) {
                move = Move::RIGHT;
                orientation = Orientation::LEFT;
            }
        } else if (data.bot.orientation == Orientation::LEFT) {
            if (data.bot.row + 1 == target.row) {
                move = Move::LEFT;
                orientation = Orientation::DOWN;
            } else if (data.bot.row - 1 == target.row) {
                move = Move::RIGHT;
                orientation = Orientation::UP;
            } else if (data.bot.col + 1 == target.col) {
                move = Move::DOWN;
                orientation = Orientation::LEFT;
            } else if (data.bot.col - 1 == target.col) {
                move = Move::UP;
                orientation = Orientation::RIGHT;
            }
        } else if (data.bot.orientation == Orientation::UP) {
            if (data.bot.row + 1 == target.row) {
                move = Move::DOWN;
                orientation = Orientation::DOWN;
            } else if (data.bot.row - 1 == target.row) {
                move = Move::UP;
                orientation = Orientation::UP;
            } else if (data.bot.col + 1 == target.col) {
                move = Move::RIGHT;
                orientation = Orientation::RIGHT;
            } else if (data.bot.col - 1 == target.col) {
                move = Move::LEFT;
                orientation = Orientation::LEFT;
            }
        }
        return make_pair(move, orientation);
    }
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    vector<string> maze = {
            {"#######"},
            {"#--#--#"},
            {"#--#--#"},
            {"#--#--#"},
            {"e-----#"},
            {"#-----#"},
            {"#######"}
    };
    int id = 0;
    Data data;

    Game game;
    Point exit = game.get_exit(maze);
    game.read_input(data);
    game.read_file(data, maze);
    game.get_orientation(data, maze);
    pair<Move, Point> next = game.get_move(data, maze);
    data.visited[next.second.row][next.second.col] = 'x';
    data.bot.row = next.second.row;
    data.bot.col = next.second.col;
    //cout << "move: " << game.move[next.first] << ", target: " << next.second.row << "," << next.second.col << endl;
    cout << game.move[next.first] << endl;
    if (next.second.row == exit.row && next.second.col == exit.col) {
        data.bot.row = 1;
        data.bot.col = 5;
        data.bot.orientation = Orientation::NONE;
        game.init_visited(data, maze);
        data.visited[1][5] = 'x';
    }
    game.write_file(data, maze);

    return 0;
}