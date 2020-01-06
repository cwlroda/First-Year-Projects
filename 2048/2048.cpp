#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>

bool proc_num(std::vector<int>& v, int bi, int ei);
void rotate_anti_clock(std::vector<int>& v);
bool game_over(const std::vector<int>& v);
void print_grid(const std::vector<int>& v);

int twod_to_oned(int row, int col, int rowlen);

bool move_up(std::vector<int>& v);
bool move_down(std::vector<int>& v);
bool move_left(std::vector<int>& v);
bool move_right(std::vector<int>& v);

void rng(std::vector<int>& v);

int main() {
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

    int num;

    while(infile >> num) {
        game.push_back(num);
    }

    print_grid(game);

    while(!game_over(game)) {
        bool valid_move = false;

        while(!valid_move) {
            std::string nextmove;
            std::cin >> nextmove;

            if(nextmove == "w") {
                valid_move = move_up(game);
            }

            else if(nextmove == "s") {
                valid_move = move_down(game);
            }

            else if(nextmove == "a") {
                valid_move = move_left(game);
            }

            else if(nextmove == "d") {
                valid_move = move_right(game);
            }
        }

        std::cout << std::endl;

        rng(game);
        print_grid(game);
    }

    std::cout << "game over" << std::endl << std::endl;
    return 0;
}

bool proc_num(std::vector<int>& v, int bi, int ei) {
    std::vector<int> vtmp = v;
    int stop = 0;

    for(int search = bi; search < ei; search++) {
        if(v[search] == 0) {
            for(int next = search+1; next < ei; next++) {
                if(v[next] != 0) {
                    if((v[next] == v[search-1]) && (search != bi) && (stop == 0)) {
                        v[search-1] *= 2;
                        search--;
                        stop = 1;
                    }

                    else {
                        v[search] = v[next];
                        stop = 0;
                    }

                    v[next] = 0;
                    next = ei;
                }
            }
        }

        else if((v[search] == v[search-1]) && (search != bi)) {
            v[search-1] *= 2;
            v[search] = 0;
            search--;
            stop = 1;
        }
    }

    if(v == vtmp) {
        return false;
    }

    return true;
}

void rotate_anti_clock(std::vector<int>& v) {
    std::vector<int> vtmp;
    int side = std::sqrt(v.size());

    for(int i = side-1; i >= 0; i--) {
        for(int j = 0; j < side; j++) {
            vtmp.push_back(v[twod_to_oned(j, i, side)]);
        }
    }

    v = vtmp;
}

bool game_over(const std::vector<int>& v) {
    int side = std::sqrt(v.size());

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

void print_grid(const std::vector<int>& v) {
    int side = std::sqrt(v.size());

    for(int i = 0; i < side; i++) {
        for(int j = 0; j < side; j++) {
            std::cout << v[twod_to_oned(i, j, side)] << "\t";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

int twod_to_oned(int row, int col, int rowlen) {
    return row*rowlen+col;
}

bool move_up(std::vector<int>& v) {
    int side = std::sqrt(v.size());
    int counter = 0;

    rotate_anti_clock(v);

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, 0, side), twod_to_oned(i, side, side))) {
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
    int side = std::sqrt(v.size());
    int counter = 0;

    for(int i = 0; i < 3; i++) {
        rotate_anti_clock(v);
    }

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, 0, side), twod_to_oned(i, side, side))) {
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
    int side = std::sqrt(v.size());
    int counter = 0;

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, 0, side), twod_to_oned(i, side, side))) {
            counter++;
        }
    }

    if(counter == side) {
        return false;
    }

    return true;
}

bool move_right(std::vector<int>& v) {
    int side = std::sqrt(v.size());
    int counter = 0;

    for(int i = 0; i < 2; i++) {
        rotate_anti_clock(v);
    }

    for(int i = 0; i < side; i++) {
        if(!proc_num(v, twod_to_oned(i, 0, side), twod_to_oned(i, side, side))) {
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

void rng(std::vector<int>& v) {
    std::srand(std::time(0));
    int i = std::rand() % v.size();

    while(v[i] != 0) {
        i = std::rand() % v.size();
    }

    v[i] = 2;
}