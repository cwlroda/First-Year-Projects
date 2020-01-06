#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <thread>
#include <ctime>

bool proc_num(std::vector<int>& v, int bi, int ei);
void rotate_anti_clock(std::vector<int>& v);
bool game_over(const std::vector<int>& v);
void print_grid(const std::vector<int>& v);

int twod_to_oned(int row, int col, int rowlen);

void next_move(std::vector<int>& v);
bool move_up(std::vector<int>& v);
bool move_down(std::vector<int>& v);
bool move_left(std::vector<int>& v);
bool move_right(std::vector<int>& v);

void rng(std::vector<int>& v);
void ai(std::string& nextmove, std::vector<int>& v, int& move);
void save_state(const std::vector<int>& v);
void new_game(const std::vector<int>& v);
void ai_two(std::string& nextmove, std::vector<int>& v);
void save_move(std::vector<std::string>& v);

int score = 0;
int scoretmp = 0;
int move = 0;
int vec = 0;
std::vector<std::string> m;
std::vector<std::string> vtemp;

int main() {
    system("clear");
    std::cout << "welcome to 2048" << std::endl;
    std::cout << "use w-a-s-d to control the board" << std::endl;
    std::cout << "type 'new' to start a new game & 'exit' to exit the game" << std::endl << std::endl;

    std::string filename;
    std::cout << "enter initial configuration file name:" << std::endl;
    std::cin >> filename;

    std::ifstream infile;
    infile.open(filename.c_str());

    std::vector<int> game;

    if(!infile.is_open()) {
        std::cout << "file not found, using default start configuration" << std::endl;

        for(int i = 0; i < 15; i++) {
            game.push_back(0);
        }

        game.push_back(2);
    }

    else{
        int num;

        while(infile >> num) {
            game.push_back(num);
        }

        infile.close();
    }

    // test if proc_num works
    /* if(!proc_num(game, 0, game.size())) {
        std::cout << "false" << std::endl;
    }

    for(int i = 0; i < game.size(); i++) {
        std::cout << game[i] << " ";
    }

    std::cout << std::endl << std::endl;
    return 0; */

    print_grid(game);
    std::cout << std::endl;

    int counter = 0;
    //srand(time(0));

    while(!game_over(game)) {
        next_move(game);
        std::cout << std::endl;

        rng(game);

        print_grid(game);
        std::cout << std::endl;

        std::cout << score << std::endl << std::endl;

        counter++;

        for(int i = 0; i < game.size(); i++) {
            if(game[i] == 2048) {
                std::cout << "you win!" << std::endl;
                std::cout << "your score is " << score << std::endl;
                std::cout << "you made " << counter << " moves" << std::endl << std::endl;

                return 0;
            }
        }
    }

    // test if rotate grid works
    /* std::cout << std::endl << std::endl;
    rotate_anti_clock(game);
    print_grid(game); */

    std::cout << "game over" << std::endl;
    std::cout << "your score is " << score << std::endl;
    std::cout << "you made " << counter << " moves" << std::endl << std::endl;

    return 0;
}

// updates grid based on move direction
bool proc_num(std::vector<int>& v, int bi, int ei) {
    std::vector<int> vtmp = v;
    int stop = 0;

    for(int i = bi; i < ei; i++) {
        if(v[i] == 0) {
            for(int j = i+1; j < ei; j++) {
                if(v[j] != 0) {
                    v[i] = v[j];

                    if((v[i] == v[i-1]) && (i != bi) && (stop == 0)) {
                        v[i-1] *= 2;
                        score += v[i-1];
                        v[i] = 0;
                        i--;
                        stop = 1;
                    }

                    else {
                        stop = 0;
                    }

                    v[j] = 0;
                    j = ei;
                }
            }
        }

        else if(v[i] == v[i-1] && i != bi) {
            v[i-1] *= 2;
            score += v[i-1];
            v[i] = 0;
            i--;
            stop = 1;
        }
    }

    int counter = 0;

    for(int i = bi; i < ei; i++) {
        if(v[i] == vtmp[i]) {
            counter++;
        }
    }

    if(counter == ei-bi) {
        return false;
    }

    return true;
}

// rotates grid anti-clockwise
void rotate_anti_clock(std::vector<int>& v) {
    std::vector<int> vtmp;
    int side = sqrt(v.size());

    for(int i = side-1; i >= 0; i--) {
        for(int j = 0; j < side; j++) {
            vtmp.push_back(v[twod_to_oned(j, i, side)]);
        }
    }

    v = vtmp;
}

// return true if game over
bool game_over(const std::vector<int>& v) {
    int side = sqrt(v.size());

    for(int i = 0; i < v.size(); i++) {
        if(v[i] == 0) {
            return false;
        }
    }

    for(int i = 0; i < side; i++) {
        for(int j = 0; j < side-1; j++) {
            if(v[twod_to_oned(i, j, side)] == v[twod_to_oned(i, j+1, side)]) {
                return false;
            }

            if(v[twod_to_oned(j, i, side)] == v[twod_to_oned(j+1, i, side)]) {
                return false;
            }
        }
    }

    return true;
}

// prints grid after every move
void print_grid(const std::vector<int>& v) {
    int side = sqrt(v.size());

    for(int i = 0; i < side; i++) {
        for(int j = 0; j < side; j++) {
            std::cout << v[twod_to_oned(i, j, side)] << "\t";
        }

        std::cout << std::endl;
    }
}

// converts 2D vector to 1D array
int twod_to_oned(int row, int col, int rowlen) {
    return row*rowlen+col;
}

// checks and performs valid move
void next_move(std::vector<int>& v) {
    std::string nextmove;
    //std::cin >> nextmove;

    ai(nextmove, v, move);

    if(nextmove == "q") {
        std::cin >> nextmove;
    }

    if(nextmove == "w") {
        if(!move_up(v)) {
            next_move(v);
        }

        std::cout << "w" << std::endl;
    }

    else if(nextmove == "s") {
        if(!move_down(v)) {
            move = 1;
            next_move(v);
        }

        std::cout << "s" << std::endl;
    }

    else if(nextmove == "a") {
        if(!move_left(v)) {
            move = 3;
            next_move(v);
        }

        std::cout << "a" << std::endl;
    }

    else if(nextmove == "d") {
        if(!move_right(v)) {
            move = 2;
            next_move(v);
        }

        std::cout << "d" << std::endl;
    }

    else if(nextmove == "exit") {
        std::string select;

        std::cout << std::endl;
        std::cout << "do you want to save (yes/no)" << std::endl;
        std::cin >> select;
        std::cout << std::endl;

        if(select == "yes") {
            save_state(v);
        }

        exit(EXIT_SUCCESS);
    }

    else if(nextmove == "new") {
        new_game(v);
    }

    else {
        next_move(v);
    }

    //m.push_back(nextmove);
    //save_move(m);
}

bool move_up(std::vector<int>& v) {
    int side = sqrt(v.size());
    int bi = 0;
    int counter = 0;

    rotate_anti_clock(v);

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, bi, side), twod_to_oned(i, side, side))) {
            counter++;
        }
    }

    for(int i = 0; i < 3; i++) {
        rotate_anti_clock(v);
    }

    if(counter == side) {
        return false;
    }

    return true;
}

bool move_down(std::vector<int>& v) {
    int side = sqrt(v.size());
    int bi = 0;
    int counter = 0;

    for(int i = 0; i < 3; i++) {
        rotate_anti_clock(v);
    }

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, bi, side), twod_to_oned(i, side, side))) {
            counter++;
        }
    }

    rotate_anti_clock(v);

    if(counter == side) {
        return false;
    }

    return true;
}

bool move_left(std::vector<int>& v) {
    int side = sqrt(v.size());
    int bi = 0;
    int counter = 0;

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, bi, side), twod_to_oned(i, side, side))) {
            counter++;
        }
    }

    if(counter == side) {
        return false;
    }

    return true;
}

bool move_right(std::vector<int>& v) {
    int side = sqrt(v.size());
    int bi = 0;
    int counter = 0;

    for(int i = 0; i < 2; i++) {
        rotate_anti_clock(v);
    }

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, bi, side), twod_to_oned(i, side, side))) {
            counter++;
        }
    }

    for(int i = 0; i < 2; i++) {
        rotate_anti_clock(v);
    }

    if(counter == side) {
        return false;
    }

    return true;
}

// new 2 random generator
void rng(std::vector<int>& v) {
    int i = rand() % v.size();
    int j = rand() % 100;

    while(v[i] != 0) {
        i++;

        if(i == v.size()) {
            i = 0;
        }
    }

    if (j >=0 && j <=9) {
        v[i] = 4;
    }

    else {
        v[i] = 2;
    }
}

void ai(std::string& nextmove, std::vector<int>& v, int& move) {
    std::string arr[2] = {"s", "d"};

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if(move == 1) {
        nextmove = "d";
        move = 0;
    }

    else if(move == 2) {
        nextmove = "a";
        move = 0;
    }

    else if(move == 3) {
        nextmove = "w";
        move = 0;
    }

    else {
        nextmove = "s";
        //nextmove = arr[rand() % 2];
    }
}

void save_state(const std::vector<int>& v) {
    std::ofstream outfile;

    std::string outfilename = "/Users/cwlroda/Desktop/C++/2048/savefile.txt";

    outfile.open(outfilename.c_str());

    if(!outfile.is_open()) {
        std::cout << "error, please create a valid save file" << std::endl << std::endl;
        save_state(v);
    }

    int side = sqrt(v.size());

    for(int i = 0; i < side; i++) {
        for(int j = 0; j < side; j++) {
            outfile << v[twod_to_oned(i, j, side)];
            outfile << "\t";
        }

        outfile << std::endl;
    }

    std::cout << "loading..." << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cout << "file successfully saved" << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    outfile.close();
}

void new_game(const std::vector<int>& v) {
    std::string select;

    std::cout << std::endl;
    std::cout << "do you want to save (yes/no)" << std::endl;
    std::cin >> select;
    std::cout << std::endl;

    if(select == "yes") {
        save_state(v);
    }

    main();
}

void save_move(std::vector<std::string>& v) {
    std::ofstream outfile;

    std::string outfilename = "/Users/cwlroda/Desktop/C++/2048/moves.txt";

    if(!outfile.is_open()) {
        outfile.open(outfilename.c_str());
    }

    for(int i = 0; i < v.size(); i++) {
        outfile << v[i] << " ";
    }
}