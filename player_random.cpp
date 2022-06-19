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
std::array<std::array<int, SIZE>, SIZE> board;

int calculate_score(string input, int who){


}

int heuristic(int who){
    int lengh = 7, score = 0;
    int dir[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    int chess_x_type[4] = {0}, chess_y_type[4] = {0};
    string chess_road;
    string type[3] = {".", "o", "x"};
    for (int i = 0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++)
        for (int k = 0; k < 4; k++){
            if (!(chess_x_type[k] & (1<<i)) && !(chess_y_type[k] & (1<<j))) continue;
            chess_road += type[board[i][j]];
            for (int l = -3; l <= 3; l++){
                if (i+dir[k][0]*l >= 0 && i+dir[k][0]*l < SIZE && j+dir[k][1]*l >= 0 && j+dir[k][1]*l < SIZE){
                    chess_x_type[k] ^= (1<<(i+dir[k][0]*l)), chess_y_type[k] ^= (1<<(j+dir[k][1]*l));
                    chess_road += type[board[i+dir[k][0]*l][j+dir[k][1]*l]];
                }
            }
            score += calculate_score(chess_road, who);
        }
    }
    return score;
}
struct state{
    int score, new_x, new_y;
    int new_chess;
    int chess_left;
    state(){
        chess_left = new_x = new_y = score = new_chess = 0;
    }
    void set_on_board(int col, int row, int who){
        new_x = col, new_y = row, new_chess = who;
        board[col][row] = player;
        score = heuristic(who);
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
state alpha_beta(state current, int depth, int alpha, int beta, int who){
    int op;
    if (player == 1) op = 2;
    else op = 1;
    if (!depth || !current.chess_left) return current;
    deque<state> all_moves = generate_all_move(player);
    if (who == player){ //1 is max
        state  max_evaluate;
        max_evaluate.score = INT_MIN;
        for (auto node:all_moves){
            state evaluate;
            board[node.new_x][node.new_y] = node.new_chess;
            node.chess_left--;
            evaluate = alpha_beta(node, depth-1, alpha, beta, op);
            board[node.new_x][node.new_y] = EMPTY;
            node.chess_left++;
            if (max_evaluate.score < evaluate.score) max_evaluate = evaluate;
            alpha = max(alpha, evaluate.score);
            if (alpha >= beta) break;
        }
        return max_evaluate;
    }
    else {
        state min_evaluate;
        min_evaluate.score = INT_MAX;
        for (auto node:all_moves){
            state evaluate;
            board[node.new_x][node.new_y] = node.new_chess;
            node.chess_left--;
            evaluate = alpha_beta(node, depth-1, alpha, beta, op);
            board[node.new_x][node.new_y] = EMPTY;
            node.chess_left++;
            if (min_evaluate.score > evaluate.score) min_evaluate = evaluate;
            beta = min(beta, evaluate.score);
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
    bool flag = false; //check whether the board is empty
    state initial;
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (board[i][j] != EMPTY){
                flag = true;
                initial.chess_left++;
            }
        }
    }
    if (!flag) x = y = 7; //if is empty，choose the middle
    else {
        state now = alpha_beta(initial, 3, INT_MIN+1, INT_MAX-1, player);
        x = now.new_x;
        y = now.new_y;
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
