//
// Created by stefa on 4/9/2020.
//
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<cmath>
#include<fstream>
using namespace std;

enum Moves { LEFT=0, RIGHT, UP, DOWN, CLEAN };

struct Point {
    int row;
    int col;
    Point() {}
    Point(int r, int c) { row = r; col = c; }
    string to_str() { return (string)("[" + to_string(row) + "," + to_string(col) + "]"); }
};

struct Game {
    vector<string> visited;
    string file = "BotCleanPOStefanosPaschalis.txt";
    Game() {}
    void init_visited(vector<string> board) {
        visited.clear();
        for (int i = 0; i < board.size(); i++) {
            string line;
            for (int j = 0; j < board[i].length(); j++) {
                line.insert(line.end(), '-');
            }
            visited.push_back(line);
        }
    }
    void write_file(Point bot, vector<string> board) {
        try {
            ofstream fout(file/*, ofstream::trunc*/);
            fout << bot.row << " " << bot.col << endl;
            for (int i = 0; i < visited.size(); i++) {
                fout << visited[i] << endl;
            }
            fout.close();
        } catch(exception e) {
            cout << e.what() << endl;
        }
    }
    void read_file(Point bot, vector<string> board) {
        try {
            ifstream fin(file);
            if (!fin.fail()) {
                int row, col;
                fin >> row >> col;
                string str = "";
                getline(fin, str);
                visited.clear();
                while(getline(fin, str)) {
                    visited.push_back(str);
                }
                fin.close();
                if (bot.row < row-1 || bot.row > row+1 || bot.col < col-1 || bot.col > col+1 ) {
                    init_visited(board);
                }
            } else {
                init_visited(board);
            }
        } catch(exception e) {
            cout << e.what() << endl;
        }
    }
    void set_visited(Point bot, vector<string> board) {
        read_file(bot, board);
        visited[bot.row][bot.col] = 'x';
        write_file(bot, board);
    }
    void print_bot(Point bot) { cout << "bot:" << bot.to_str() << endl; }
    void print_target(Point target) { cout << "target:" << target.to_str() << endl; }
    void print_board(Point bot, vector<string> board, vector<string> visited) {
        vector<string> vboard = get_visible_board(bot, board);
        for(int i=0; i<board.size(); i++) {
            string line = vboard[i] + "  |  " + board[i] + "  |  " + visited[i];
            cout << line << endl;
        }
    }
    void print_targets(vector<Point> targets) {
        cout << targets.size() << " targets -> ";
        for (int i = 0; i < targets.size(); i++) {
            cout << i << ":" << targets[i].to_str() << ", ";
        }
        cout << endl;
    }
    void print_distances(multimap<float,Point> distances) {
        cout << "distances: ";
        for(auto distance : distances) {
            cout << distance.second.to_str() << "->" << distance.first << ", ";
        }
        cout << endl;
    }
    void print_move(Moves move) {
        string str[5] = { {"LEFT"}, {"RIGHT"}, {"UP"}, {"DOWN"}, {"CLEAN"} };
        cout << "Move:" << str[move] << endl;
    }
    vector<string> get_visible_board(Point bot, vector<string> board) {
        string line;
        vector<string> vboard;
        for (int i = 0; i < board.size(); i++) {
            line = "";
            for (int j = 0; j < board[i].length(); j++) {
                if (i >= (bot.row - 1) && i <= (bot.row + 1) && j >= (bot.col - 1) && j <= (bot.col + 1)) {
                    if (i == bot.row && j == bot.col) {
                        if (board[i][j] == '-') {
                            line.insert(line.end(), 'b');
                        } else {
                            line.insert(line.end(), board[i][j]);
                        }
                    } else {
                        line.insert(line.end(), board[i][j]);
                    }
                } else {
                    line.insert(line.end(), 'o');
                }
            }
            vboard.push_back(line);
        }
        return vboard;
    }
    //vector<Point> get_targets(Point bot, vector<string> board, vector<string> visited) {
    vector<Point> get_targets(Point bot, vector<string> board) {
        vector<Point> targets;
        for(int i=0; i<board.size(); i++) {
            int pos = 0;
            size_t yp = board[i].find('d', pos);
            while(yp != string::npos) {
                targets.push_back(Point(i, yp));
                pos = yp + 1;
                yp = board[i].find('d', pos);
            }
        }
        if (targets.empty()) {
            for(int i=0; i<visited.size(); i++) {
                int pos = 0;
                size_t yp = visited[i].find('-', pos);
                while(yp != string::npos) {
                    targets.push_back(Point(i, yp));
                    pos = yp + 1;
                    yp = visited[i].find('-', pos);
                }
            }
        }
        return targets;
    }
    multimap<float,Point> get_distances(Point bot, vector<Point> targets) {
        float h = .0;
        float d = .0;
        multimap<float, Point, less<float>> distances;
        for(int i=0; i<targets.size(); i++) {
            float h = .0;
            for(int j=0; j<targets.size(); j++) {
                h += sqrt(pow(targets[i].row - targets[j].row, 2) + pow(targets[i].col - targets[j].col, 2));
            }
            d = sqrt(pow(bot.row - targets[i].row, 2) + pow(bot.col-targets[i].col, 2)) / h;
            distances.insert(make_pair(d, targets[i]));
        }
        return distances;
    }
    Moves get_move(Point bot, Point target) {
        Moves move;
        int v_dist = bot.row - target.row;
        int h_dist = bot.col - target.col;
        Moves v_direction = (bot.row >= target.row ? Moves::UP : Moves::DOWN);
        Moves h_direction = (bot.col >= target.col ? Moves::LEFT : Moves::RIGHT);
        if (v_dist !=0 && h_dist != 0) {
            if (abs(v_dist) <= abs(h_dist)) {
                move = v_direction;
            } else {
                move = h_direction;
            }
        } else if (v_dist ==0 && h_dist != 0) {
            move = h_direction;
        } else if (v_dist !=0 && h_dist == 0) {
            move = v_direction;
        } else if (v_dist ==0 && h_dist == 0) {
            move = Moves::CLEAN;
        }
        return move;
    }
    void play(Point &bot, vector<string> &board) {
        int moves = 0;
        int cleans = 0;
        bool play = true;
        Point target;

        while(play==true) {
            cout << "----------------------" << endl;
            moves++;
            print_bot(bot);
            set_visited(bot, board);
            print_board(bot, board, visited);

            vector<string> visible_board = get_visible_board(bot, board);
            //vector<Point> targets = get_targets(bot, visible_board, visited);
            vector<Point> targets = get_targets(bot, visible_board);
            print_targets(targets);

            multimap<float,Point> distances = get_distances(bot, targets);
            print_distances(distances);

            target = distances.begin()->second;
            print_target(target);

            Moves move = get_move(bot, target);
            print_move(move);

            board[bot.row][bot.col] = '-';
            if (move == Moves::UP) bot.row--;
            else if (move == Moves::DOWN) bot.row++;
            else if (move == Moves::LEFT) bot.col--;
            else if (move == Moves::RIGHT) bot.col++;
            else if (move == Moves::CLEAN) {
                board[target.row][target.col] = 'b';
                cleans++;
            }

            int cntr = 0;
            for(int i=0; i<board.size(); i++) {
                for(int j=0; j<board[i].length(); j++) {
                    if (board[i][j] == 'd' || board[i][j] == 'o') cntr++;
                }
            }
            if (cntr == 0) {
                play = false;
            }
        }
        cout << "total moves: " << moves << endl;
        cout << "total cleans: " << cleans << endl;
    }
};

void next_move(int posr, int posc, vector <string> board) {
    Game game;
    Point bot(posr, posc);
    game.set_visited(bot, board);
    vector<Point> targets = game.get_targets(bot, board);
    multimap<float,Point> distances = game.get_distances(bot, targets);
    Point target = distances.begin()->second;
    Moves move = game.get_move(bot, target);
    string str[5] = { {"LEFT"}, {"RIGHT"}, {"UP"}, {"DOWN"}, {"CLEAN"} };
    cout << str[move] << endl;
}

int main() {
    int row = 0;
    int col = 0;
    vector<string> board;
    board.push_back("----d");
    board.push_back("-----");
    board.push_back("-----");
    board.push_back("-----");
    board.push_back("-----");

    Game game;
    Point bot(row, col);
    game.play(bot, board);

    /*vector<string> board;
    //0 0
    board.push_back("bdooo");
    board.push_back("-dooo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    next_move(0, 0, board);
    board.clear();


    //0 1
    board.push_back("-d-oo");
    board.push_back("-d-oo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    next_move(0, 1, board);
    board.clear();

    //0 1
    board.push_back("-b-oo");
    board.push_back("-d-oo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    next_move(0, 1, board);
    board.clear();

    //1 1
    board.push_back("---oo");
    board.push_back("-d-oo");
    board.push_back("---oo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    next_move(1, 1, board);
    board.clear();

    //1 1
    board.push_back("---oo");
    board.push_back("-b-oo");
    board.push_back("---oo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    next_move(1, 1, board);
    board.clear();

    //1 0
    board.push_back("--ooo");
    board.push_back("b-ooo");
    board.push_back("--ooo");
    board.push_back("ooooo");
    board.push_back("ooooo");
    next_move(1, 0, board);
    board.clear();*/

    return 0;
}