//
// Created by stefa on 4/1/2020.
//
#include <iostream>
#include <vector>
using namespace std;

void displayPathtoPrincess(int n, vector <string> grid){
    //your logic here
    //Find bot & princess
    pair<int, int> bot_pos, princess_pos;
    for(int i=0; i<grid.size(); i++) {
        size_t yb = grid[i].find('m', 0);
        if (yb != string::npos) {
            bot_pos = make_pair(i, yb);
        }
        size_t yp = grid[i].find('p', 0);
        if (yp != string::npos) {
            princess_pos = make_pair(i, yp);
        }
    }

    //Get path
    int v_distance = bot_pos.first - princess_pos.first;
    string v_step = (bot_pos.first >= princess_pos.first ? "UP" : "DOWN");
    for(int i=0; i<abs(bot_pos.first - princess_pos.first); i++) {
        cout << v_step << endl;
    }
    int h_distance = bot_pos.second - princess_pos.second;
    string h_step = (bot_pos.second >= princess_pos.second ? "LEFT" : "RIGHT");
    for(int i=0; i<abs(bot_pos.second - princess_pos.second); i++) {
        cout << h_step << endl;
    }
}

int main(void) {

    int m;
    vector <string> grid;

    cin >> m;

    for(int i=0; i<m; i++) {
        string s; cin >> s;
        grid.push_back(s);
    }

    displayPathtoPrincess(m,grid);

    return 0;
}

