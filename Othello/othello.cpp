#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

vector<string> transcript;
vector<vector<string>> allstates;
unordered_map<string, pair<int, int>> directions;

int run();
void reset();
void welcome();

void default_grid(vector<string>& board);
void print_grid(vector<string> &board);
void build_directions();

bool next_move(vector<string>& board, string& piece, string& nextpiece);
bool placer(int row, int col, vector<string>& board, string& piece, string& nextpiece);

bool game_over(vector<string>& board, string& piece, string& nextpiece);
bool pass_move(vector<string>& board, string& piece, string& nextpiece);
bool available_move(vector<string>& board, string& piece, string& nextpiece);
void possible_moves(vector<string>& board, string & piece);

int twod_to_oned(int row, int col, int rowlen);

void flip(int row, int col, vector<string> &board, string &piece, string &nextpiece, pair<string, pair<int, int>> dir);
void switch_player(string& piece, string& nextpiece);

void move_score(vector<string>& board);
void print_score(vector<string> &board);
void print_moves();
void record_move(vector<string> &transcript, string &move, string &piece);
void viewAnalysis();

int run() {
    welcome();

    vector<string> board;
    string piece = "   O   ";
    string nextpiece = "   X   ";
    bool valid_move = false;

    build_directions();
    default_grid(board);
    print_grid(board);
    move_score(board);
    allstates.push_back(board);

    while (!game_over(board, piece, nextpiece)) {
        while (!valid_move) {
            valid_move = next_move(board, piece, nextpiece);

            welcome();
            print_grid(board);

            if (!valid_move) {
                cout << "Invalid move!" << endl << endl;
            }

            move_score(board);
        }

        switch_player(piece, nextpiece);
        valid_move = false;
    }

    cout << "Game Over!" << endl << endl;
    print_score(board);

    cout << "Enter N to start a new game, V to view analysis, or any other key to quit" << endl;
    string option;
    cin >> option;

    if (option == "n" || option == "N") {
        return 1;
    }

    else if (option == "v" || option == "V") {
        viewAnalysis();
    }

    return 0;
}

void reset() {
    transcript.clear();
    allstates.clear();
}

void welcome() {
    system("clear");
    cout << "Welcome to Othello" << endl;
    cout << "Player 1 is O, Player 2 is X" << endl;
    cout << "Enter board notation as next move" << endl;
    cout << "e.g. A1 places a piece on the top left corner square" << endl;
    cout << "Type 'undo' to undo move" << endl << endl;
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

void build_directions() {
    directions["N"] = make_pair(-1, 0);
    directions["NE"] = make_pair(-1, 1);
    directions["E"] = make_pair(0, 1);
    directions["SE"] = make_pair(1, 1);
    directions["S"] = make_pair(1, 0);
    directions["SW"] = make_pair(1, -1);
    directions["W"] = make_pair(0, -1);
    directions["NW"] = make_pair(-1, -1);
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
                for (const auto &dir : directions) {
                    flip(i, j, boardtmp, piece, nextpiece, dir);
                }
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
    if (piece == "   O   ") {
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

    for (const auto &dir : directions) {
        flip(row, col, board, piece, nextpiece, dir);
    }

    if (boardtmp == board) {
        return false;
    }

    return true;
}

bool flipCheck(int row, int col, string dir) {
    if (dir == "N") {return row > 1;}
    else if (dir == "NE") {return row > 1 && col < 6;}
    else if (dir == "E") {return col < 6;}
    else if (dir == "SE") {return row < 6 && col < 6;}
    else if (dir == "S") {return row < 6;}
    else if (dir == "SW") {return row < 6 && col > 1;}
    else if (dir == "W") {return col > 1;}
    else if (dir == "NW") {return row > 1 && col > 1;}

    return false;
}

void flip(int row, int col, vector<string> &board, string &piece, string &nextpiece, pair<string, pair<int,int>> dir) {
    bool check = false;
    int rowtmp = row;
    int coltmp = col;

    int roffset = dir.second.first;
    int coffset = dir.second.second;

    while (flipCheck(row, col, dir.first) &&
           (board[twod_to_oned(row + (roffset * 2), col + (coffset * 2), 8)] != "\t")) {
        if ((board[twod_to_oned(row + (roffset * 2), col + (coffset * 2), 8)] == piece) &&
            (board[twod_to_oned(rowtmp + roffset, coltmp + coffset, 8)] == nextpiece)) {
            check = true;
            break;
        }

        row += roffset;
        col += coffset;
    }

    if (check) {
        board[twod_to_oned(rowtmp, coltmp, 8)] = piece;

        while (flipCheck(rowtmp, coltmp, dir.first) &&
               (board[twod_to_oned(rowtmp + roffset, coltmp + coffset, 8)] != piece) &&
               (board[twod_to_oned(rowtmp + roffset, coltmp + coffset, 8)] != "\t")) {
            board[twod_to_oned(rowtmp + roffset, coltmp + coffset, 8)] = piece;
            rowtmp += roffset;
            coltmp += coffset;
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

    cout << "O :\t" << ocount << endl;
    cout << "X :\t" << xcount << endl << endl;
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

    cout << "Player 1 (O) has " << xcount << " pieces" << endl;
    cout << "Player 2 (X) has " << ocount << " pieces" << endl << endl;

    if (xcount > ocount) {
        cout << "Player 1 (O) wins!";
    }

    else if (ocount > xcount) {
        cout << "Player 2 (X) wins!";
    }

    else {
        cout << "It's a draw!";
    }

    cout << endl << endl;
    print_moves();
    cout << endl << endl << endl;
}

void print_moves() {
    cout << "Moves:" << endl;

    string filename = "moves.txt";
    ofstream outfile;

    outfile.open(filename.c_str());

    for (int i = 0; i < transcript.size(); i += 2) {
        cout << (i / 2) + 1 << "\t" << transcript[i] << "\t" << transcript[i+1] << endl;
        outfile << (i / 2) + 1 << "\t" << transcript[i] << "\t" << transcript[i+1] << endl;
    }

    outfile.close();
}

void record_move(vector<string>& transcript, string& move, string& piece) {
    if (move[0] >= 97 && move[0] <= 104) {
        move[0] -= 32;
    }

    if (move != "undo") {
        transcript.push_back(move);
    }
}

void viewAnalysis() {
    int state = allstates.size() - 1;
    string option;

    system("clear");
    cout << "Analysis" << endl;
    cout << "Enter N to view next move, and P to view previous move" << endl;
    cout << "========" << endl << endl;

    print_grid(allstates[state]);

    do {
        cin >> option;

        if (option == "n" || option == "N") {
            if (state < allstates.size() - 1) {
                state++;
            }
        }

        else if (option == "p" || option == "P") {
            if (state > 0) {
                state--;
            }
        }

        system("clear");
        cout << "Analysis" << endl;
        cout << "Enter N to view next move, P to view previous move, and Q to exit" << endl;
        cout << "========" << endl << endl;

        print_grid(allstates[state]);
    } while (option != "q" || option != "Q");
}

int main() {
    while (run()) {
        reset();
    };

    return 0;
}
