//
// Created by stefa on 4/13/2020.
//
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

enum Move { RIGHT=0, UP, LEFT, DOWN };
typedef Move Orientation;

struct Point {
    int row;
    int col;
    Point() {}
    Point(int r, int c) { row = r; col = c; }
};

struct Slice {
    int top;
    int left;
    Point bot;
    Orientation orientation;
    vector<string> slice;
    Slice() {}
};

struct Game {
    Game() {}
    void print_board(vector<string> board) {
        cout << "----- BOARD -----" << endl;
        for(int i=0; i<board.size(); i++) {
            cout << board[i] << endl;
        }
    }
    void print_pattern(vector<string> pattern) {
        for(int i=0; i<pattern.size(); i++) {
            cout << pattern[i] << endl;
        }
    }
    string orientation_to_str(Orientation orientation) {
        string str[4] = { "RIGHT", "UP", "LEFT", "DOWN"};
        return str[orientation];
    }
    void print_slice(Slice slice) {
        cout << "----- SLICE -----" << endl;
        cout << "top,left: " << slice.top << "," << slice.left << endl;
        print_pattern(slice.slice);
        cout << "bot row,col: " << slice.bot.row << "," << slice.bot.col << endl;
        cout << "bot orientation: " << orientation_to_str(slice.orientation) << endl;
    }
    void rotate(vector<string> &board) {
        vector<string> rotated;
        for(int j=0; j<board[0].length(); j++) {
            string row = "";
            for (int i = board.size()-1; i >= 0; i--) {
                row.insert(row.end(), board[i][j]);
            }
            rotated.push_back(row);
        }
        board.empty();
        board = rotated;
    }
    vector<string> get_slice(vector<string> board, int row, int col) {
        vector<string> result;
        for(int i=row; i<row+3; i++) {
            result.push_back(board[i].substr(col, 3));
        }
        return result;
    }
    vector<Slice> get_slices(vector<string>board, vector<string>pattern) {
        vector<Slice> slices;
        for(int i=0; i<=board.size()-3; i++) {
            for(int j=0; j<=board[i].length()-3; j++) {
                Slice slice;
                slice.top = i;
                slice.left = j;
                slice.slice = get_slice(board, i, j);
                slices.push_back(slice);
            }
        }
        return slices;
    }
    vector<Slice> find_patterns(vector<Slice> slices, vector<string> pattern) {
        vector<Slice> found_slices;
        for(int i=0; i<slices.size(); i++) {
            if (slices[i].slice == pattern) {
                found_slices.push_back(slices[i]);
            }
        }
        return found_slices;
    }
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    vector<string> board = {
        {"#######"},
        {"#--#--#"},
        {"#--#--#"},
        {"#--#--#"},
        {"e-----#"},
        {"#-----#"},
        {"#######"}
        /*{"########"},
        {"#---#--#"},
        {"#---#--#"},
        {"#---#--#"},
        {"e------#"},
        {"#------#"},
        {"########"}*/
    };

    Point bot(1, 5);

    vector<string> input = {
        {"###"},
        {"#--"},
        {"#--"}
    };

    Game game;

    int rotations = 0;
    while(rotations < 4) {
        game.print_board(board);
        cout << endl;
        vector<Slice> slices = game.get_slices(board, input);
        vector<Slice> found_slices = game.find_patterns(slices, input);
        if (!found_slices.empty()) {
            for(int i=0; i<found_slices.size(); i++) {
                found_slices[i].orientation = (Orientation) rotations;
                found_slices[i].bot.row = found_slices[i].top + 1;
                found_slices[i].bot.col = found_slices[i].left + 1;
                game.print_slice(found_slices[i]);
            }
        }
        game.rotate(board);
        rotations++;
    }

    /*
    vector<string> input = {
            {"###"},
            {"--#"},
            {"--#"}
    };
    /*vector<string> input = {
            {"--#"},
            {"--#"},
            {"###"}
    };
    vector<string> input = {
            {"#--"},
            {"#--"},
            {"###"}
    };

    game.rotate(board);
    game.print_board(board);
    cout << endl;

    game.rotate(board);
    game.print_board(board);
    cout << endl;

    game.rotate(board);
    game.print_board(board);
    cout << endl;

    game.rotate(board);
    game.print_board(board);
    cout << endl;*/

    return 0;
}