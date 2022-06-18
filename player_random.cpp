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

int player;
const int SIZE = 15;
int chess_left = SIZE*SIZE;
std::array<std::array<int, SIZE>, SIZE> board;
int v = 1000;
int heuristic(){
    v++;
    return v;
}
struct state{
    bool game_end, board_empty;
    int score, new_x, new_y;
    int new_chess;
    state(){
        board_empty = false;
        game_end = false;
        new_x = new_y = 0;
        score = 0;
        new_chess = 0;
    }
    void set_on_board(int col, int row, int who){
        new_x = col, new_y = row, new_chess = who;
        chess_left--;
        board[col][row] = player;
        score = heuristic();
    }
};

deque<state> generate_all_move(int who){
    int dir[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1 ,-1}};
    deque<state> all_possible_move;
    for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++){
        if (board[i][j] != EMPTY){
            for (int k = 0; k < 8; k++){
                if (dir[k][0]+i >= 0 && dir[k][0]+i < SIZE && dir[k][1]+j >= 0 && dir[k][1]+i < SIZE && board[dir[k][0]+i][dir[k][1]+j] == EMPTY){
                    state tmp;
                    tmp.set_on_board(dir[k][0]+i, dir[k][1]+j, who);
                    all_possible_move.emplace_back(tmp);
                }
            }
        }
    }
    return all_possible_move;
}

//(score, (x, y))
pair<int, pair<int, int> > alpha_beta(state current, int depth, int alpha, int beta, int who){
    int op;
    if (player == 1) op = 2;
    else op = 1;
    if (!depth){
        pair<int, pair<int, int>> heur_score;
        heur_score.first = current.score;
        heur_score.second.first = current.new_x, heur_score.second.second = current.new_y;
        board[current.new_x][current.new_y] = EMPTY;
        chess_left++;
        return heur_score;
    }
    deque<state> all_moves = generate_all_move(player);
    if (who == player){ //1 is max
        pair<int, pair<int, int>>  max_evaluate;
        max_evaluate.first = INT_MIN;
        for (auto node:all_moves){
            pair<int, pair<int, int>> evaluate;
            board[node.new_x][node.new_y] = node.new_chess;
            evaluate = alpha_beta(node, depth-1, alpha, beta, op);
            board[node.new_x][node.new_y] = EMPTY;
            if (max_evaluate.first < evaluate.first){
                max_evaluate = evaluate;
            }
            alpha = max(alpha, evaluate.first);
            if (alpha >= beta) break;
        }
        return max_evaluate;
    }
    else {
        pair<int, pair<int, int>> min_evaluate;
        min_evaluate.first = INT_MAX;
        for (auto node:all_moves){
            pair<int, pair<int, int>> evaluate;
            board[node.new_x][node.new_y] = node.new_chess;
            evaluate = alpha_beta(node, depth-1, alpha, beta, op);
            board[node.new_x][node.new_y] = EMPTY;
            if (min_evaluate.first > evaluate.first){
                min_evaluate = evaluate;
            }
            beta = min(beta, evaluate.first);
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
    int x, y;
    // Keep updating the output until getting killed.
    bool flag = false;
    state initial;
    // Choose a random spot.
    if (!flag) {
        for (int i = 0; i < SIZE && !flag; i++){
            for (int j = 0; j < SIZE; j++){
                if (board[i][j] != EMPTY){
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)
            x = y = 7;
    }
    if (flag){
        pair<int, pair<int, int>> now = alpha_beta(initial, 4, INT_MIN+1, INT_MAX-1, player);
        x = now.second.first;
        y = now.second.second;
    }
    if (board[x][y] == EMPTY) {
        fout << x << " " << y << std::endl;
        // Remember to flush the output to ensure the last action is written to file.
        fout.flush();
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
