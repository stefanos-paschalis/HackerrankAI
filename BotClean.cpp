#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<cmath>

using namespace std;

enum Moves {
    LEFT=0,
    RIGHT,
    UP,
    DOWN,
    CLEAN
};

struct Point {
    int row;
    int col;
    Point *next;
    Point(int r, int c) { row = r; col = c; next = NULL; }
    string to_str() { return (string)("[" + to_string(row) + "," + to_string(col) + "]"); }
};

struct Game {
    vector<vector<Point>> paths;
    void print_bot(Point bot) { cout << "bot:" << bot.to_str() << endl; }
    void print_board(vector<string> board) {
        for (int i = 0; i < board.size(); i++) {
            cout << board[i] << endl;
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
    int get_board_size(vector<string> board) {
        int size = 0;
        for(int i=0; i<board.size(); i++) {
            size += board[i].length();
        }
        return size;
    }
    vector<Point> get_targets(vector<string> board) {
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
    vector<Moves> get_moves(Point bot, vector<string> board) {
        vector<Moves> moves;
        int num_moves = 0;
        vector<Point> targets = get_targets(board);
        multimap<float,Point> distances = get_distances(bot, targets);
        Point target = distances.begin()->second;
        while(!targets.empty()) {
            print_board(board);
            print_bot(bot);
            print_targets(targets);
            print_distances(distances);
            cout << "selected target:" << target.to_str() << endl;

            Moves move = get_move(bot, target);
            if (move == Moves::UP) {
                num_moves++;
                board[bot.row][bot.col] = '-';
                bot.row--;
                if (board[bot.row][bot.col] != 'd') board[bot.row][bot.col] = 'm';
            }
            else if (move == Moves::DOWN) {
                num_moves++;
                board[bot.row][bot.col] = '-';
                bot.row++;
                if (board[bot.row][bot.col] != 'd') board[bot.row][bot.col] = 'm';
            }
            else if (move == Moves::LEFT) {
                num_moves++;
                board[bot.row][bot.col] = '-';
                bot.col--;
                if (board[bot.row][bot.col] != 'd') board[bot.row][bot.col] = 'm';
            }
            else if (move == Moves::RIGHT) {
                num_moves++;
                board[bot.row][bot.col] = '-';
                bot.col++;
                if (board[bot.row][bot.col] != 'd') board[bot.row][bot.col] = 'm';
            }
            else if (move == Moves::CLEAN) {
                board[target.row][target.col] = 'm';
                targets = get_targets(board);
                distances = get_distances(bot, targets);
                target = distances.begin()->second;
            }
            print_move(move);
            cout << "moves so far: " << num_moves << endl;
            moves.push_back(move);
            cout << "----------------------" << endl;
        }
        cout << "number of moves without clean: " << num_moves << endl;
        cout << "number of moves with clean: " << moves.size() << endl;
        return moves;
    }
};

void next_move(int posr, int posc, vector <string> board) {
    //add logic here
    Point bot(posr, posc);
    Game game;
    vector<Point> targets = game.get_targets(board);
    multimap<float,Point> distances = game.get_distances(bot, targets);
    Point target = distances.begin()->second;
    Moves move = game.get_move(bot, target);
    string str[5] = { {"LEFT"}, {"RIGHT"}, {"UP"}, {"DOWN"}, {"CLEAN"} };
    cout << str[move] << endl;
}

int main(void) {
    int pos[2];
    vector <string> board;
    cin>>pos[0]>>pos[1];
    for(int i=0;i<5;i++) {
        string s;cin >> s;
        board.push_back(s);
    }
    next_move(pos[0], pos[1], board);
    return 0;
}
