#include <iostream>
#include <random>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include <list>
#include <iterator>

void set_random_seed();
int randn(int n);
void default_attempt(std::vector<int>& attempt, int length, int num);

struct mm_code_maker{

    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
    }

    void generate_sequence(){
        for(int i = 0; i < length; i++){
            sequence.push_back(randn(num));
        }
    }

    void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
        black_hits = 0;
        white_hits = 0;

        std::vector<int> stmp = sequence;
        std::vector<int> atmp = attempt;

        for(int i = 0; i < length; i++){
            if(atmp[i] == stmp[i]){
                black_hits++;
                atmp[i] = -1;
                stmp[i] = -1;
            }
        }

        for(int i = 0; i < length; i++){
            for(int j = 0; j < length; j++){
                if((atmp[i] == stmp[j]) && (atmp[i] != -1) && (stmp[j] != -1)){
                    white_hits++;
                    atmp[i] = -1;
                    stmp[j] = -1;

                    j = stmp.size();
                }
            }
        }
    }

    std::vector<int> sequence;

    int length;
    int num;
};

struct mm_solver{

    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;

        if((length > 15) || (num > 15)){
            num_moves = 5764802;
        }

        else{
            num_moves = pow(num, length);
        }

        if(num_moves < 5764802 && !(length == 14 && num == 3)){
            for(int i = 0; i < num; i++) {
                nums.push_back(i);
            }

            std::vector<int> pos(length, 0);
            genAllMoves(pos, 0);

            if(num_moves < 7777){
                seq = all_seq;
            }
        }

        for(int i = 1; i <= length+1; i++){
            comb += i;
        }
    }

    void create_attempt(std::vector<int>& attempt){
        if(!check){
            default_attempt(attempt, length, num);
            check = true;
        }

        else if((length == 4) && (num == 6)){
            miniMax(attempt);
        }

        else if(num_moves < 5764802 && !(length == 14 && num == 3)) {
            if(num_moves < 7777){
                if(all_seq.size() == 1){
                    it = all_seq.begin();
                    attempt = *it;
                }

                else{
                    maxParts(attempt);
                }
            }

            else{
                it = all_seq.begin();
                attempt = *it;
            }
        }
    }

    void learn(std::vector<int>& attempt, int black_hits, int white_hits){
        if(num_moves < 5764802 && !(length == 14 && num == 3)){
            it = all_seq.begin();

            while(it != all_seq.end()) {
                std::vector<int> atmp = attempt;
                std::vector<int> mtmp = *it;
                int black = 0, white = 0;

                for(int j = 0; j < length; j++) {
                    if(atmp[j] == mtmp[j]) {
                        black++;
                        atmp[j] = -1;
                        mtmp[j] = -1;
                    }
                }

                for(int i = 0; i < length; i++){
                    for(int j = 0; j < length; j++){
                        if((atmp[i] == mtmp[j]) && (atmp[i] != -1) && (mtmp[j] != -1)) {
                            white++;
                            atmp[i] = -1;
                            mtmp[j] = -1;

                            j = mtmp.size();
                        }
                    }
                }

                if((black != black_hits) || (white != white_hits)){
                    it = all_seq.erase(it);
                }

                else{
                    it++;
                }

                if(attempt == *it) {
                    it = all_seq.erase(it);
                }
            }
        }

        else{
            if(black_hits > prev_black){
                counter++;
            }

            int ind = length-counter;
            itSearch(attempt, ind, black_hits);
            prev_black = black_hits;
        }
    }

    void miniMax(std::vector<int>& attempt){
        std::vector<int> score;
        it = seq.begin();

        while(it != seq.end()){
            if(attempt == *it){
                it = seq.erase(it);
            }

            std::vector<int> hitcount(comb-1, 0);
            std::list<std::vector<int> >::iterator itt = all_seq.begin();

            while(itt != all_seq.end()){
                std::vector<int> atmp = *it;
                std::vector<int> mtmp = *itt;
                int black = 0, white = 0;

                for(int i = 0; i < length; i++){
                    if(atmp[i] == mtmp[i]){
                        black++;
                        atmp[i] = -1;
                        mtmp[i] = -1;
                    }
                }

                for(int i = 0; i < length; i++){
                    for(int j = 0; j < length; j++){
                        if((atmp[i] == mtmp[j]) && (atmp[i] != -1) && (mtmp[j] != -1)){
                            white++;
                            atmp[i] = -1;
                            mtmp[j] = -1;

                            j = mtmp.size();
                        }
                    }
                }

                int elem = 0;

                for(int i = 1; i <= black; i++){
                    elem += length+2-i;
                }

                for(int i = 1; i <= white; i++){
                    elem++;
                }

                if(black == length){
                    elem--;
                }

                hitcount[elem]++;
                itt++;
            }

            int seqsize = all_seq.size();
            int scoring = seqsize;

            for(int i = 0; i < hitcount.size(); i++){
                if((seqsize - hitcount[i]) < scoring){
                    scoring = seqsize - hitcount[i];
                }
            }

            score.push_back(scoring);

            it++;
        }

        std::list<std::vector<int> >::iterator itmp = seq.begin();
        int count = 0;
        bool guess = false;

        for(int i = 0; i < score.size(); i++){
            if(score[i] > count){
                attempt = *itmp;
                count = score[i];

                it = all_seq.begin();

                while(it != all_seq.end()){
                    if(*itmp == *it){
                        guess = true;
                        it = all_seq.end();
                    }

                    else{
                        guess = false;
                        it++;
                    }
                }
            }

            else if((score[i] == count) && (guess == false)){
                it = all_seq.begin();

                while(it != all_seq.end()){
                    if(*itmp == *it){
                        attempt = *itmp;
                        guess = true;
                        it = all_seq.end();
                    }

                    else{
                        it++;
                    }
                }
            }

            itmp++;
        }
    }

    void maxParts(std::vector<int>& attempt){
        std::vector<std::vector<int> > empty;
        std::vector<std::vector<std::vector<int> > > parts(comb, empty);

        it = seq.begin();

        while(it != seq.end()){
            if(attempt == *it){
                it = seq.erase(it);
            }

            std::vector<int> hitcount(comb-1, 0);
            std::list<std::vector<int> >::iterator itt = all_seq.begin();

            while(itt != all_seq.end()){
                std::vector<int> atmp = *it;
                std::vector<int> mtmp = *itt;
                int black = 0, white = 0;

                for(int i = 0; i < length; i++){
                    if(atmp[i] == mtmp[i]){
                        black++;
                        atmp[i] = -1;
                        mtmp[i] = -1;
                    }
                }

                for(int i = 0; i < length; i++){
                    for(int j = 0; j < length; j++){
                        if((atmp[i] == mtmp[j]) && (atmp[i] != -1) && (mtmp[j] != -1)){
                            white++;
                            atmp[i] = -1;
                            mtmp[j] = -1;

                            j = mtmp.size();
                        }
                    }
                }

                int elem = 0;

                for(int i = 1; i <= black; i++){
                    elem += length+2-i;
                }

                for(int i = 1; i <= white; i++){
                    elem++;
                }

                if(black == length){
                    elem--;
                }

                hitcount[elem]++;
                itt++;
            }

            int score = 0;

            for(int i = 0; i < hitcount.size(); i++){
                if(hitcount[i] > 0){
                    score++;
                }
            }

            std::vector<int> guess = *it;
            parts[score].push_back(guess);

            it++;
        }

        for(int i = parts.size()-1; i >= 0; i--){
            if(parts[i].size() > 0){
                attempt = parts[i][0];

                for(int j = 0; j < parts[i].size(); j++){
                    it = all_seq.begin();

                    while(it != all_seq.end()){
                        if(parts[i][j] == *it){
                            attempt = parts[i][j];
                            it = all_seq.end();
                            j = parts[i].size();
                        }

                        else{
                            it++;
                        }
                    }
                }

                i = 0;
            }
        }
    }

    void itSearch(std::vector<int>& attempt, int ind, int black_hits){
        if((black_hits < prev_black) && (prev_black != 10000)){
            attempt[ind]--;
        }

        else{
            attempt[ind]++;
        }

        if(attempt[ind] > num-1){
            attempt[ind] = 0;
        }
    }

    void genAllMoves(std::vector<int>& pos, int n){
        std::vector<int> tmp(length, 0);

        if(n == length){
            for (int i = 0; i < n; i++){
                tmp[i] = nums[pos[i]];
            }

            all_seq.insert(all_seq.end(), tmp);
            return;
        }

        for(int i = 0; i < nums.size(); i++){
            pos[n] = i;
            genAllMoves(pos, n+1);
        }
    }

    int length;
    int num;

    int comb = 0;
    int counter = 1;
    int prev_black = 10000;
    long long int num_moves;

    bool check = false;

    std::vector<int> nums;
    std::list<std::vector<int> > all_seq, seq;
    std::list<std::vector<int> >::iterator it;
};

int main(){
    set_random_seed();

    int length, num;
    std::cout << "enter length of sequence and number of possible values:" << std::endl;
    std::cin >> length >> num;
    std::cout << std::endl;

    double start_time = clock();

    mm_solver solver;
    solver.init(length, num);

    mm_code_maker maker;
    maker.init(length, num);
    maker.generate_sequence();

    int black_hits = 0, white_hits = 0;
    int attempts = 1;

    std::vector<int> attempt;
    solver.create_attempt(attempt);

    while(black_hits < length) {
        std::cout << "attempt " << attempts << ":" << std::endl;

        for(int i = 0; i < attempt.size(); i++) {
            std::cout << attempt[i] << " ";
        }

        std::cout << std::endl << std::endl;
        maker.give_feedback(attempt, black_hits, white_hits);
        std::cout << "black pegs: " << black_hits << std::endl;
        std::cout << "white pegs: " << white_hits << std::endl << std::endl;

        if(black_hits != length) {
            solver.learn(attempt, black_hits, white_hits);
            solver.create_attempt(attempt);

            attempts++;
        }
    }

    std::cout << "the solver has found the sequence in " << attempts << " attempts" << std::endl;
    std::cout << "the sequence generated by the code maker was:" << std::endl;

    for(int i = 0; i < maker.sequence.size(); i++) {
        std::cout << maker.sequence[i] << " ";
    }

    std::cout << std::endl << std::endl;

    double end_time = clock();
    std::cout << "time taken: " << (end_time - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl << std::endl;

    return 0;
}

void set_random_seed(){
    std::srand(time(0));
}

int randn(int n){
    return std::rand() % n;
}

void default_attempt(std::vector<int>& attempt, int length, int num){
    if(num == 1){
        for(int i = 0; i < length; i++){
            attempt.push_back(0);
        }
    }

    else{
        if(length == 2){
            attempt.push_back(0);
            attempt.push_back(1);
        }

        else if(length == 3){
            if(num <= 3){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
            }

            else{
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(2);
            }
        }

        else if(length == 4) {
            if((num <= 3) || (num == 6)){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
            }

            else{
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(2);
            }
        }

        else if(length == 5){
            if(num == 2){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
            }

            if(num == 3){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
            }

            else{
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
                attempt.push_back(2);
            }
        }

        else if(length == 6){
            if(num == 2){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
            }

            if(num == 3){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
                attempt.push_back(1);
            }

            else{
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
                attempt.push_back(2);
            }
        }

        else if(length == 7){
            if(num == 2){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
            }

            else{
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
                attempt.push_back(1);
                attempt.push_back(2);
            }
        }

        else if(length == 8){
            if(num == 2){
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
            }

            else{
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(0);
                attempt.push_back(1);
                attempt.push_back(1);
                attempt.push_back(1);
                attempt.push_back(2);
            }
        }

        else{
            for(int i = 0; i < length; i++){
                attempt.push_back(0);
            }
        }
    }
}