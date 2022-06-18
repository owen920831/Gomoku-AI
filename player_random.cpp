#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <bits/stdc++.h>
using namespace std;
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

struct state{
    bool game_end;
    int score;
    int chess_left;
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

deque<state> generate_all_move(){
    deque<state> all_possible_move;
    return all_possible_move;
}

void heuristic(){

}
int alpha_beta(state current, int depth, int alpha,int beta, bool maximzingplayer){
    if (!depth || current.game_end|| !current.chess_left){
        return current.score;
    }
    deque<state> all_moves = generate_all_move();
    if (maximzingplayer){ //1 is max
        int max_evaluate = INT_MIN;
        for (auto node:all_moves){
            max_evaluate = max(max_evaluate, alpha_beta(node, depth-1, alpha, beta, false));
            alpha = max(alpha, max_evaluate);
            if (alpha >= beta) break;
        }
        return max_evaluate;
    }
    else {
        int min_evaluate = INT_MAX;
        for (auto node:all_moves){
            min_evaluate = min(min_evaluate, alpha_beta(node, depth-1, alpha, beta, true));
            beta = min(beta, min_evaluate);
            if (alpha >= beta) break;
        }
        return min_evaluate;
    }
}

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    //int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
