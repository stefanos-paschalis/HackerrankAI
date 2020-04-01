//
// Created by stefa on 4/1/2020.
//
#include<iostream>
#include<vector>
#include <cmath>
using namespace std;

void next_move(int posr, int posc, vector <string> board) {
    //add logic here
    static bool target_found = false;
    static pair<int, int> target;
    pair<int, int> bot_pos = make_pair(posr, posc);

    //Get all targets
    vector<pair<int,int>> targets;
    for(int i=0; i<board.size() && target_found==false; i++) {
        size_t yp = board[i].find('d', 0);
        if (yp != string::npos) {
            targets.push_back(make_pair(i, yp));
        }
    }

    //Find the closest target
    if (target_found == false) {
        int min_index = 0;
        float min_distance = 100.0;
        for (int i = 0; i < targets.size(); i++) {
            float distance = sqrt(pow(bot_pos.first - targets[i].first, 2)
            +pow((bot_pos.second - targets[i].second), 2));
            if (distance <= min_distance) {
                min_distance = distance;
                min_index = i;
            }
        }
        target = make_pair(targets[min_index].first, targets[min_index].second);
        target_found = true;
    }

    int v_distance = bot_pos.first - target.first;
    string v_step = (bot_pos.first >= target.first ? "UP" : "DOWN");
    int h_distance = bot_pos.second - target.second;
    string h_step = (bot_pos.second >= target.second ? "LEFT" : "RIGHT");
    if (v_distance != 0 && h_distance != 0) {
        if (abs(v_distance) <= abs(h_distance)) {
            cout << v_step << endl;
        } else {
            cout << h_step << endl;
        }
    } else if (v_distance == 0 && h_distance != 0) {
        cout << h_step << endl;
    } else if (v_distance != 0 && h_distance == 0) {
        cout << v_step << endl;
    } else if (v_distance==0 && h_distance == 0) {
        target_found = false;
        cout << "CLEAN";
    }
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

