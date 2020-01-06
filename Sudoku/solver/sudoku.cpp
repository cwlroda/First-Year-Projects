#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define size 9

int twod_to_oned(int row, int col, int rowlen);
bool solve(vector<int>& board);
bool empty_sq(vector<int> board, int& row, int& col);
bool is_safe(vector<int> board, int row, int col, int num);
bool check_row(vector<int> board, int row, int num);
bool check_col(vector<int> board, int col, int num);
bool check_subsq(vector<int> board, int rstart, int cstart, int num);

int main(int argc, char** argv){
    ifstream infile;
    infile.open(argv[1]);

    if(!infile.is_open()){
        cout << "Error opening sudoku.txt!" << endl;
        exit(EXIT_FAILURE);
    }

    vector<int> board;
    int n;

    while(infile >> n){
        board.push_back(n);
    }

    infile.close();

    solve(board);

    system("clear");

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            cout << board[twod_to_oned(i, j, size)] << " ";
        }

        cout << endl;
    }

    cout << endl;
    cout << "Sudoku is solved!" << endl;

    return 0;
}

int twod_to_oned(int row, int col, int rowlen){
    return row*rowlen+col;
}

bool solve(vector<int>& board){
    system("clear");

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            cout << board[twod_to_oned(i, j, size)] << " ";
        }

        cout << endl;
    }

    int row, col;

    if(!empty_sq(board, row, col)){
        return true;
    }

    for(int num=1; num<10; num++){
        if(is_safe(board, row, col, num)){
            board[twod_to_oned(row, col, size)] = num;

            if(solve(board)){
                return true;
            }

            board[twod_to_oned(row, col, size)] = 0;
        }
    }

    return false;
}

bool empty_sq(vector<int> board, int& row, int& col){
    for(row=0; row<size; row++){
        for(col=0; col<size; col++){
            if(board[twod_to_oned(row, col, size)] == 0){
                return true;
            }
        }
    }

    return false;
}

bool is_safe(vector<int> board, int row, int col, int num){
    if(check_row(board, row, num) && check_col(board, col, num) && check_subsq(board, row-row%3, col-col%3, num) && empty_sq(board, row, col)){
        return true;
    }

    return false;
}

bool check_row(vector<int> board, int row, int num){
    for(int col=0; col<size; col++){
        if(board[twod_to_oned(row, col, size)] == num){
            return false;
        }
    }

    return true;
}

bool check_col(vector<int> board, int col, int num){
    for(int row=0; row<size; row++){
        if(board[twod_to_oned(row, col, size)] == num){
            return false;
        }
    }

    return true;
}

bool check_subsq(vector<int> board, int rstart, int cstart, int num){
    for(int row=0; row<3; row++){
        for(int col=0; col<3; col++){
            if(board[twod_to_oned(row+rstart, col+cstart, size)] == num){
                return false;
            }
        }
    }

    return true;
}