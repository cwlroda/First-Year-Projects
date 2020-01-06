#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

vector<string> transcript;
vector<vector<string> > allstates;

void default_grid(vector<string>& board);
void print_grid(vector<string>& board);

bool next_move(vector<string>& board, string& piece, string& nextpiece);
bool placer(int row, int col, vector<string>& board, string& piece, string& nextpiece);

bool game_over(vector<string>& board, string& piece, string& nextpiece);
bool pass_move(vector<string>& board, string& piece, string& nextpiece);
bool available_move(vector<string>& board, string& piece, string& nextpiece);
void possible_moves(vector<string>& board, string & piece);

int twod_to_oned(int row, int col, int rowlen);

void flipN(int row, int col, vector<string>& board, string& piece, string& nextpiece);
void flipNE(int row, int col, vector<string>& board, string& piece, string& nextpiece);
void flipE(int row, int col, vector<string>& board, string& piece, string& nextpiece);
void flipSE(int row, int col, vector<string>& board, string& piece, string& nextpiece);
void flipS(int row, int col, vector<string>& board, string& piece, string& nextpiece);
void flipSW(int row, int col, vector<string>& board, string& piece, string& nextpiece);
void flipW(int row, int col, vector<string>& board, string& piece, string& nextpiece);
void flipNW(int row, int col, vector<string>& board, string& piece, string& nextpiece);

void switch_player(string& piece, string& nextpiece);

void move_score(vector<string>& board);
void print_score(vector<string>& board);
void record_move(vector<string>& transcript, string& move, string& piece);

int main() {
    system("clear");

    cout << "Welcome to Othello" << endl;
    cout << "Player 1 is X, Player 2 is O" << endl;
    cout << "Enter board notation as next move" << endl;
    cout << "e.g. A1 places a piece on the top left corner square" << endl;
    cout << "Type 'undo' to undo move" << endl << endl;

    vector<string> board;
    string piece = "   X   ";
    string nextpiece = "   O   ";
    bool valid_move = false;

    default_grid(board);
    print_grid(board);
    move_score(board);
    allstates.push_back(board);

    while (!game_over(board, piece, nextpiece)) {
        while (!valid_move) {
            valid_move = next_move(board, piece, nextpiece);

            system("clear");
            cout << "Welcome to Othello" << endl;
            cout << "Player 1 is X, Player 2 is O" << endl;
            cout << "Enter board notation as next move" << endl;
            cout << "e.g. A1 places a piece on the top left corner square" << endl;
            cout << "Type 'undo' to undo move" << endl << endl;

            print_grid(board);
            move_score(board);
        }

        switch_player(piece, nextpiece);
        valid_move = false;
    }

    cout << "Game Over!" << endl << endl;
    print_score(board);

    return 0;
}

void default_grid(vector<string>& board) {
    for (int i = 0; i < 64; i++) {
        if (i == 27 || i == 36) {
            board.push_back("   X   ");
        }

        else if (i == 28 || i == 35) {
            board.push_back("   O   ");
        }

        else {
            board.push_back("\t");
        }
    }
}

void print_grid(vector<string>& board) {
    cout << "\t    A       B       C       D       E       F       G       H" << endl;

    for (int i = 0; i < 8; i++) {
        cout << "\t-----------------------------------------------------------------";
        cout << endl;

        cout << i+1 << "\t";

        for (int j = 0; j < 8; j++) {
            cout << "|" << board[twod_to_oned(i, j, 8)];
        }

        cout << "|";
        cout << endl;
    }

    cout << "\t-----------------------------------------------------------------";
    cout << endl << endl;
}

bool game_over(vector<string>& board, string& piece, string& nextpiece) {
    int xcount = 0;
    int ocount = 0;

    for (int i = 0; i < 64; i++) {
        if (board[i] == "   X   ") {
            xcount++;
        }

        else if (board[i] == "   O   ") {
            ocount++;
        }
    }

    if (xcount == 0 || ocount == 0) {
        return true;
    }

    if (pass_move(board, piece, nextpiece)) {
        return true;
    }

    return false;
}

bool pass_move(vector<string>& board, string& piece, string& nextpiece) {
    if (available_move(board, piece, nextpiece)) {
        if (available_move(board, nextpiece, piece)) {
            return true;
        }

        switch_player(piece, nextpiece);
        cout << "Pass!" << endl << endl;
    }

    return false;
}

bool available_move(vector<string>& board, string& piece, string& nextpiece) {
    vector<string> boardtmp = board;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[twod_to_oned(i, j, 8)] == "\t") {
                flipN(i, j, boardtmp, piece, nextpiece);
                flipNE(i, j, boardtmp, piece, nextpiece);
                flipE(i, j, boardtmp, piece, nextpiece);
                flipSE(i, j, boardtmp, piece, nextpiece);
                flipS(i, j, boardtmp, piece, nextpiece);
                flipSW(i, j, boardtmp, piece, nextpiece);
                flipW(i, j, boardtmp, piece, nextpiece);
                flipNW(i, j, boardtmp, piece, nextpiece);
            }
        }
    }

    if (boardtmp == board) {
        return true;
    }

    return false;
}

void possible_moves(vector<string>& board, string & piece) {

}

int twod_to_oned(int row, int col, int rowlen) {
    return row*rowlen+col;
}

bool next_move(vector<string>& board, string& piece, string& nextpiece) {
    if (piece == "   X   ") {
        cout << "Player 1's turn:" << endl << endl;
    }

    else {
        cout << "Player 2's turn" << endl << endl;
    }

    cout << piece << "\t";
    string move;
    cin >> move;
    cout << endl;

    int row, col, counter;

    if (move == "undo" || move == "Undo" || move == "UNDO") {
        if (allstates.size() > 1) {
            allstates.pop_back();
            board = allstates[allstates.size() - 1];

            transcript.pop_back();
            record_move(transcript, move, piece);

            return true;
        }

        else {
            return false;
        }
    }

    if (move.length() != 2) {
        return false;
    }

    if (move[0] >= 97 && move[0] <= 104) {
        col = move[0] - 97;
    }

    else if (move[0] >= 65 && move[0] <= 72) {
        col = move[0] - 65;
    }

    else {
        return false;
    }

    if (move[1] >= 49 && move[1] <= 56) {
        row = move[1] - 49;
    }

    else {
        return false;
    }

    if (!placer(row, col, board, piece, nextpiece)) {
        return false;
    }

    record_move(transcript, move, piece);
    allstates.push_back(board);

    return true;
}

bool placer(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    vector<string> boardtmp = board;

    if (row == 0) {
        if (col == 0) {
            if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row+1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col+1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col+1, 8)] == nextpiece))) {
                flipE(row, col, board, piece, nextpiece);
                flipSE(row, col, board, piece, nextpiece);
                flipS(row, col, board, piece, nextpiece);
            }

            else {
                return false;
            }
        }

        else if (col == 7) {
            if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row+1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col-1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col-1, 8)] == nextpiece))) {
                flipS(row, col, board, piece, nextpiece);
                flipSW(row, col, board, piece, nextpiece);
                flipW(row, col, board, piece, nextpiece);
            }

            else {
                return false;
            }
        }

        else {
            if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row+1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col-1, 8)] == nextpiece) || (board[twod_to_oned(row, col+1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col-1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col+1, 8)] == nextpiece))) {
                flipE(row, col, board, piece, nextpiece);
                flipSE(row, col, board, piece, nextpiece);
                flipS(row, col, board, piece, nextpiece);
                flipSW(row, col, board, piece, nextpiece);
                flipW(row, col, board, piece, nextpiece);
            }

            else {
                return false;
            }
        }
    }

    else if (col == 0) {
        if (row == 7) {
            if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row-1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col+1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col+1, 8)] == nextpiece))) {
                flipN(row, col, board, piece, nextpiece);
                flipNE(row, col, board, piece, nextpiece);
                flipE(row, col, board, piece, nextpiece);
            }

            else {
                return false;
            }
        }

        else {
            if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row-1, col, 8)] == nextpiece) || (board[twod_to_oned(row+1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col+1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col+1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col+1, 8)] == nextpiece))) {
                flipN(row, col, board, piece, nextpiece);
                flipNE(row, col, board, piece, nextpiece);
                flipE(row, col, board, piece, nextpiece);
                flipSE(row, col, board, piece, nextpiece);
                flipS(row, col, board, piece, nextpiece);
            }

            else {
                return false;
            }
        }
    }

    else if (row == 7) {
        if (col == 7) {
            if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row-1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col-1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col-1, 8)] == nextpiece))) {
                flipW(row, col, board, piece, nextpiece);
                flipNW(row, col, board, piece, nextpiece);
                flipN(row, col, board, piece, nextpiece);
            }

            else {
                return false;
            }
        }

        else {
            if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row-1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col-1, 8)] == nextpiece) || (board[twod_to_oned(row, col+1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col-1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col+1, 8)] == nextpiece))) {
                flipW(row, col, board, piece, nextpiece);
                flipNW(row, col, board, piece, nextpiece);
                flipN(row, col, board, piece, nextpiece);
                flipNE(row, col, board, piece, nextpiece);
                flipE(row, col, board, piece, nextpiece);
            }

            else {
                return false;
            }
        }
    }

    else if (col == 7) {
        if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row-1, col, 8)] == nextpiece) || (board[twod_to_oned(row+1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col-1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col-1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col-1, 8)] == nextpiece))) {
            flipS(row, col, board, piece, nextpiece);
            flipSW(row, col, board, piece, nextpiece);
            flipW(row, col, board, piece, nextpiece);
            flipNW(row, col, board, piece, nextpiece);
            flipN(row, col, board, piece, nextpiece);
        }

        else {
            return false;
        }
    }

    else {
        if ((board[twod_to_oned(row, col, 8)] == "\t") && ((board[twod_to_oned(row-1, col, 8)] == nextpiece) || (board[twod_to_oned(row+1, col, 8)] == nextpiece) || (board[twod_to_oned(row, col-1, 8)] == nextpiece) || (board[twod_to_oned(row, col+1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col-1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col-1, 8)] == nextpiece) || (board[twod_to_oned(row-1, col+1, 8)] == nextpiece) || (board[twod_to_oned(row+1, col+1, 8)] == nextpiece))) {
            flipN(row, col, board, piece, nextpiece);
            flipNE(row, col, board, piece, nextpiece);
            flipE(row, col, board, piece, nextpiece);
            flipSE(row, col, board, piece, nextpiece);
            flipS(row, col, board, piece, nextpiece);
            flipSW(row, col, board, piece, nextpiece);
            flipW(row, col, board, piece, nextpiece);
            flipNW(row, col, board, piece, nextpiece);
        }

        else {
            return false;
        }
    }

    if (boardtmp == board) {
        return false;
    }

    return true;
}

void flipN(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (rowtmp > 1 && (board[twod_to_oned(rowtmp-2, coltmp, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp-2, coltmp, 8)] == piece) && (board[twod_to_oned(rowtmp2-1, coltmp2, 8)] == nextpiece)) {
            check = true;
            break;
        }

        rowtmp--;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2-1, coltmp2, 8)] != piece) && (board[twod_to_oned(rowtmp2-1, coltmp2, 8)] != "\t") && (rowtmp2 > 1)) {
            board[twod_to_oned(rowtmp2-1, coltmp2, 8)] = piece;
            rowtmp2--;
        }
    }
}

void flipNE(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (rowtmp > 1 && coltmp < 6 && (board[twod_to_oned(rowtmp-2, coltmp+2, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp-2, coltmp+2, 8)] == piece) && (board[twod_to_oned(rowtmp2-1, coltmp2+1, 8)] == nextpiece)) {
            check = true;
            break;
        }

        rowtmp--;
        coltmp++;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2-1, coltmp2+1, 8)] != piece) && (board[twod_to_oned(rowtmp2-1, coltmp2+1, 8)] != "\t") && (rowtmp2 > 1 && coltmp2 < 6)) {
            board[twod_to_oned(rowtmp2-1, coltmp2+1, 8)] = piece;
            rowtmp2--;
            coltmp2++;
        }
    }
}

void flipE(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (coltmp < 6 && (board[twod_to_oned(rowtmp, coltmp+2, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp, coltmp+2, 8)] == piece) && (board[twod_to_oned(rowtmp2, coltmp2+1, 8)] == nextpiece)) {
            check = true;
            break;
        }

        coltmp++;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2, coltmp2+1, 8)] != piece) && (board[twod_to_oned(rowtmp2, coltmp2+1, 8)] != "\t") && (coltmp2 < 6)) {
            board[twod_to_oned(rowtmp2, coltmp2+1, 8)] = piece;
            coltmp2++;
        }
    }
}

void flipSE(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (rowtmp < 6 && coltmp < 6 && (board[twod_to_oned(rowtmp+2, coltmp+2, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp+2, coltmp+2, 8)] == piece) && (board[twod_to_oned(rowtmp2+1, coltmp2+1, 8)] == nextpiece)) {
            check = true;
            break;
        }

        rowtmp++;
        coltmp++;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2+1, coltmp2+1, 8)] != piece) && (board[twod_to_oned(rowtmp2+1, coltmp2+1, 8)] != "\t") && (rowtmp2 < 6 && coltmp2 < 6)) {
            board[twod_to_oned(rowtmp2+1, coltmp2+1, 8)] = piece;
            rowtmp2++;
            coltmp2++;
        }
    }
}

void flipS(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (rowtmp < 6 && (board[twod_to_oned(rowtmp+2, coltmp, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp+2, coltmp, 8)] == piece) && (board[twod_to_oned(rowtmp2+1, coltmp2, 8)] == nextpiece)) {
            check = true;
            break;
        }

        rowtmp++;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2+1, coltmp2, 8)] != piece) && (board[twod_to_oned(rowtmp2+1, coltmp2, 8)] != "\t") && (rowtmp2 < 6)) {
            board[twod_to_oned(rowtmp2+1, coltmp2, 8)] = piece;
            rowtmp2++;
        }
    }
}

void flipSW(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (rowtmp < 6 && coltmp > 1 && (board[twod_to_oned(rowtmp+2, coltmp-2, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp+2, coltmp-2, 8)] == piece) && (board[twod_to_oned(rowtmp2+1, coltmp2-1, 8)] == nextpiece)) {
            check = true;
            break;
        }

        rowtmp++;
        coltmp--;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2+1, coltmp2-1, 8)] != piece) && (board[twod_to_oned(rowtmp2+1, coltmp2-1, 8)] != "\t") && (rowtmp2 < 6 && coltmp2 > 1)) {
            board[twod_to_oned(rowtmp2+1, coltmp2-1, 8)] = piece;
            rowtmp2++;
            coltmp2--;
        }
    }
}

void flipW(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (coltmp > 1 && (board[twod_to_oned(rowtmp, coltmp-2, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp, coltmp-2, 8)] == piece) && (board[twod_to_oned(rowtmp2, coltmp2-1, 8)] == nextpiece)) {
            check = true;
            break;
        }

        coltmp--;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2, coltmp2-1, 8)] != piece) && (board[twod_to_oned(rowtmp2, coltmp2-1, 8)] != "\t") && (coltmp2 > 1)) {
            board[twod_to_oned(rowtmp2, coltmp2-1, 8)] = piece;
            coltmp2--;
        }
    }
}

void flipNW(int row, int col, vector<string>& board, string& piece, string& nextpiece) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;
    int rowtmp2 = row;
    int coltmp2 = col;

    while (rowtmp > 1 && coltmp > 1 && (board[twod_to_oned(rowtmp-2, coltmp-2, 8)] != "\t")) {
        if ((board[twod_to_oned(rowtmp-2, coltmp-2, 8)] == piece) && (board[twod_to_oned(rowtmp2-1, coltmp2-1, 8)] == nextpiece)) {
            check = true;
            break;
        }

        rowtmp--;
        coltmp--;
    }

    if (check) {
        board[twod_to_oned(rowtmp2, coltmp2, 8)] = piece;

        while ((board[twod_to_oned(rowtmp2-1, coltmp2-1, 8)] != piece) && (board[twod_to_oned(rowtmp2-1, coltmp2-1, 8)] != "\t") && (rowtmp2 > 1 && coltmp2 > 1)) {
            board[twod_to_oned(rowtmp2-1, coltmp2-1, 8)] = piece;
            rowtmp2--;
            coltmp2--;
        }
    }
}

void switch_player(string& piece, string& nextpiece) {
    string tmp;
    tmp = piece;
    piece = nextpiece;
    nextpiece = tmp;
}

void move_score(vector<string>& board) {
    int xcount = 0;
    int ocount = 0;

    for (int i = 0; i < 64; i++) {
        if (board[i] == "   X   ") {
            xcount++;
        }

        else if (board[i] == "   O   ") {
            ocount++;
        }
    }

    cout << "X :\t" << xcount << endl;
    cout << "O :\t" << ocount << endl << endl;
}

void print_score(vector<string>& board) {
    int xcount = 0;
    int ocount = 0;

    for (int i = 0; i < 64; i++) {
        if (board[i] == "   X   ") {
            xcount++;
        }

        else if (board[i] == "   O   ") {
            ocount++;
        }
    }

    cout << "Player 1 (X) has " << xcount << " pieces" << endl;
    cout << "Player 2 (O) has " << ocount << " pieces" << endl << endl;

    if (xcount > ocount) {
        cout << "Player 1 (X) wins!";
    }

    else if (ocount > xcount) {
        cout << "Player 2 (O) wins!";
    }

    else if (xcount == ocount) {
        cout << "It's a draw!";
    }

    cout << endl << endl << endl;
}

void record_move(vector<string>& transcript, string& move, string& piece) {
    string filename = "moves.txt";
    ofstream outfile;

    outfile.open(filename.c_str());

    if (move[0] >= 97 && move[0] <= 104) {
        move[0] -= 32;
    }

    if (move != "undo") {
        if (piece == "   X   ") {
            transcript.push_back("X\t" + move);
        }

        else if (piece == "   O   ") {
            transcript.push_back("O\t" + move);
        }
    }

    for (int i = 0; i < transcript.size(); i++) {
        outfile << i+1 << "\t" << transcript[i] << endl;
    }

    outfile.close();
}