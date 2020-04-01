//
// Created by stefa on 4/1/2020.
//
#include <iostream>
#include <vector>
using namespace std;

void nextMove(int n, int r, int c, vector <string> grid){
    //your logic here
    //Find bot & princess
    pair<int, int> bot_pos, princess_pos;
    bot_pos = make_pair(r, c);
    for(int i=0; i<grid.size(); i++) {
        size_t yp = grid[i].find('p', 0);
        if (yp != string::npos) {
            princess_pos = make_pair(i, yp);
        }
    }

    //Get path
    int v_distance = bot_pos.first - princess_pos.first;
    string v_step = (bot_pos.first >= princess_pos.first ? "UP" : "DOWN");
    int h_distance = bot_pos.second - princess_pos.second;
    string h_step = (bot_pos.second >= princess_pos.second ? "LEFT" : "RIGHT");
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
    }
}

int main(void) {

    int n, r, c;
    vector <string> grid;

    cin >> n;
    cin >> r;
    cin >> c;

    for(int i=0; i<n; i++) {
        string s; cin >> s;
        grid.push_back(s);
    }

    nextMove(n, r, c, grid);
    return 0;
}
