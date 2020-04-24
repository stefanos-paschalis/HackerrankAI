//
// Created by stefa on 4/24/2020.
//
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <cstdio>
using namespace std;

enum Move { NONE=-1, UP, LEFT, DOWN, RIGHT };
typedef Move Orientation;

struct Point {
    int id;
    int row;
    int col;
    Move move;
    Orientation orientation;
    Point() {}
    Point(int r, int c) { row = r; col = c; }
};

struct Board {
    int top;
    int left;
    vector<string> cells;
    void print_cells() {
        for(int i=0; i<cells.size(); i++) {
            cout << cells[i] << endl;
        }
    }
};

struct Data {
    Point bot;
    Point exit;
    Board board;
    vector<string> visited;
    Data() {}
    void print_visited() {
        for(int i=0; i<visited.size(); i++) {
            cout << visited[i] << endl;
        }
    }
};

struct Game {
    Game() {}
    const string filename1 = "MazeEscapeBot1.txt";
    const string filename2 = "MazeEscapeBot2.txt";
    const string orientation[4] = { "UP", "LEFT", "DOWN", "RIGHT" };
    const string move[4] = { "UP", "LEFT", "DOWN", "RIGHT" };
    void init_bot(Point &bot) {
        //bot.id = 0;
        bot.row = 1;
        bot.col = 5;
        bot.move = NONE;
        bot.orientation = NONE;
    }
    void init_board(Board &board) {
        board.left = -1;
        board.top = -1;
        board.cells.clear();
    }
    void init_visited(vector<string> &visited, vector<string> &maze) {
        visited.clear();
        for (int i = 0; i < maze.size(); i++) {
            string line;
            for (int j=0; j<maze[i].length(); j++) {
                line.insert(line.end(), maze[i][j]);
            }
            visited.push_back(line);
        }
    }
    void init_data(Data &data, vector<string> &maze) {
        init_bot(data.bot);
        //init_board(data.board);
        init_visited(data.visited, maze);
        data.visited[data.bot.row][data.bot.col] = 'x';
    }
    void read_console(Data &data) {
        cin >> data.bot.id;
        for(int i=0; i<3; i++) {
            string str;
            cin >> str;
            data.board.cells.push_back(str);
        }
    }
    void read_file(Data &data, vector<string> &maze) {
        try {
            ifstream fin;
            if (data.bot.id == 1) {
                fin.open(filename1);
            } else if (data.bot.id == 2) {
                fin.open(filename2);
            }
            if (!fin.fail()) {
                int orientation;
                fin >> data.bot.row >> data.bot.col >> orientation;
                data.bot.orientation = (Orientation)orientation;
                string str;
                getline(fin, str);
                data.visited.clear();
                while(getline(fin, str)) {
                    data.visited.push_back(str);
                }
                fin.close();
            } else {
                init_data(data, maze);
            }
        } catch(exception e) {
            cout << e.what() << endl;
        }
    }
    void write_file(Data &data) {
        try {
            ofstream fout;
            if (data.bot.id == 1) {
                fout.open(filename1);
            } else if (data.bot.id == 2) {
                fout.open(filename2);
            }
            fout << data.bot.row << " " << data.bot.col << " " << data.bot.orientation << endl;
            for (int i = 0; i < data.visited.size(); i++) {
                fout << data.visited[i] << endl;
            }
            fout.close();
        } catch(exception e) {
            cout << e.what() << endl;
        }
    }
    void delete_file(Data &data) {
        if (data.bot.id == 1) {
            remove( filename1.c_str() );
        } else if (data.bot.id == 2) {
            remove( filename2.c_str() );
        }
    }
    void get_exit(Data &data, vector<string> &maze) {
        int row = -1, col = -1;
        for (int i = 0; i < maze.size(); i++) {
            for (int j = 0; j < maze[i].length(); j++) {
                if (maze[i][j] == 'e') {
                    row = i;
                    col = j;
                }
            }
        }
        data.exit.row = row;
        data.exit.col = col;
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
                data.board.top = data.bot.row - 1;
                data.board.left = data.bot.col - 1;
            }
            rotate_clockwise(adjacent_cells);
            rotations++;
        }
    }
    pair<Move, Orientation> get_move(Data &data, Point target) {
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
    Point get_target(Data &data) {
        multimap<float, Point, less<float>> distances;
        for(int i=data.bot.row-1; i<=data.bot.row+1; i++) {
            for (int j = data.bot.col - 1; j < data.bot.col + 1; j++) {
                if (data.visited[i][j] == 'e') {
                    return Point(i, j);
                } else if (data.visited[i][j] == '-'
                           && (i == data.bot.row || j == data.bot.col)) {
                    float distance = sqrt(pow(data.exit.row-i,2) + pow(data.exit.col-j,2));
                    distances.insert(make_pair(distance, Point(i, j)));
                }
            }
        }
        return distances.begin()->second;
    }
    void do_move(Data &data, Point target) {
        data.bot.row = target.row;
        data.bot.col = target.col;
        data.visited[target.row][target.col] = 'x';
    }
    void next_input(Data &data, pair<Move, Orientation> &move, vector<string> &maze) {
        vector<string> cells;
        for(int i=data.bot.row-1; i<=data.bot.row+1; i++) {
            cells.push_back(maze[i].substr(data.bot.col-1, 3));
        }
        if (move.second == Orientation::LEFT) {
            rotate_clockwise(cells);
        } else if (move.second == Orientation::DOWN) {
            rotate_clockwise(cells);
            rotate_clockwise(cells);
        } else if (move.second == Orientation::RIGHT) {
            rotate_counter_clockwise(cells);
        } else if (move.second == Orientation::UP) {
            rotate_counter_clockwise(cells);
            rotate_counter_clockwise(cells);
        }
        data.board.cells = cells;
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
    Data data;
    Game game;
    game.read_console(data);
    cout << "------------------------------------" << endl;
    game.read_file(data, maze);
    game.get_exit(data, maze);
    game.get_orientation(data, maze);
    cout << "bot position: " << data.bot.row << ", " << data.bot.col << endl;
    cout << "bot orientation: " << game.orientation[data.bot.orientation] << endl;
    Point target = game.get_target(data);
    cout << "next position: " << target.row << ", " << target.col << endl;
    pair<Move, Orientation> move = game.get_move(data, target);
    cout << "next move: " << game.move[move.first] << ", next orientation: " << game.orientation[move.second]
         << endl;
    game.do_move(data, target);
    if (data.bot.row != data.exit.row && data.bot.col != data.exit.col) {
        game.next_input(data, move, maze);
        data.print_visited();
        data.board.print_cells();
        game.write_file(data);
    } else {
        game.delete_file(data);
    }

    return 0;
}

