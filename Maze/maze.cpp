#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <random>

struct node {
	char id;
	node* north;
	node* south;
	node* east;
	node* west;
};

typedef node* nodeptr;

void build_basic_maze(nodeptr& start, nodeptr& finish);
// builds the basic maze displayed in page 1

nodeptr traverse_maze(const nodeptr& start, const std::string& path);
// traverses the maze using a predefined path

void solve_interactively(const nodeptr& start, const nodeptr& finish, std::string& path);
// allows the user to solve the maze interactively

nodeptr random_walk(const nodeptr& start, const nodeptr& finish, std::string& path);
// simulates a random walk in the maze

void solve_queue(const nodeptr& start, const nodeptr& finish, std::string& path);
// an improvement of the random walk: solves the maze using a queue to store the unvisited neighbours of the current node

void solve_stack(const nodeptr& start, const nodeptr& finish, std::string& path);
// an improvement of the random walk: solves the maze using a stack to store the unvisited neighbours of the current node

bool bfs(const nodeptr& start, const nodeptr& finish, std::string& path, nodeptr& current, nodeptr& prev, std::vector<nodeptr>& visitednodes, std::vector<nodeptr>& tmp, int height);

int main() {
    srand(time(0));

    nodeptr start = NULL;
    nodeptr finish = NULL;

    build_basic_maze(start, finish);
    nodeptr current = start;

    std::string path;

    solve_interactively(start, finish, path);
    std::cout << std::endl;

    std::string path2;
    current = random_walk(start, finish, path2);
    std::cout << path2 << std::endl;

    if (current == finish) {
        std::cout << "Congratulations, you reached the finish!" << std::endl << std::endl;
    }

    else {
        std::cout << "Sorry, you have reached the maximum number of moves (20)" << std::endl;
        std::cout << "You reached room " << current->id << std::endl << std::endl;
    }

    std::string path3;
    solve_queue(start, finish, path3);

    std::string path4;
    solve_stack(start, finish, path4);

	return 0;
}

void build_basic_maze(nodeptr& start, nodeptr& finish){
    nodeptr A = new node, B = new node,
            C = new node, D = new node,
            E = new node, F = new node,
            G = new node, H = new node,
            I = new node, J = new node,
            K = new node, L = new node;

    A->id = 'A',
    A->north = NULL,
    A->south = E,
    A->east = B,
    A->west = NULL;

    B->id = 'B',
    B->north = NULL,
    B->south = F,
    B->east = NULL,
    B->west = A;

    C->id = 'C',
    C->north = NULL,
    C->south = G,
    C->east = D,
    C->west = NULL;

    D->id = 'D',
    D->north = NULL,
    D->south = NULL,
    D->east = NULL,
    D->west = C;

    E->id = 'E',
    E->north = A,
    E->south = I,
    E->east = NULL,
    E->west = NULL;

    F->id = 'F',
    F->north = B,
    F->south = NULL,
    F->east = G,
    F->west = NULL;

    G->id = 'G',
    G->north = C,
    G->south = K,
    G->east = H,
    G->west = F;

    H->id = 'H',
    H->north = NULL,
    H->south = L,
    H->east = NULL,
    H->west = G;

    I->id = 'I',
    I->north = E,
    I->south = NULL,
    I->east = J,
    I->west = NULL;

    J->id = 'J',
    J->north = NULL,
    J->south = NULL,
    J->east = NULL,
    J->west = I;

    K->id = 'K',
    K->north = G,
    K->south = NULL,
    K->east = NULL,
    K->west = NULL;

    L->id = 'L',
    L->north = H,
    L->south = NULL,
    L->east = NULL,
    L->west = NULL;

    start = A;
    finish = L;
}

nodeptr traverse_maze(const nodeptr& start, const std::string& path){
    nodeptr stop = start;

    for(int i = 0; i < path.size(); i++){
        if((path[i] == 'N' || path[i] == 'n') && (stop->north != NULL)) {stop = stop->north;}
        else if((path[i] == 'S' || path[i] == 's') && (stop->south != NULL)) {stop = stop->south;}
        else if((path[i] == 'E' || path[i] == 'e') && (stop->east != NULL)) {stop = stop->east;}
        else if((path[i] == 'W' || path[i] == 'w') && (stop->west != NULL)) {stop = stop->west;}
    }

    return stop;
}

void solve_interactively(const nodeptr& start, const nodeptr& finish, std::string& path){
    nodeptr current = start;

    while(current != finish){
        std::cout << "You are in room " << current->id << " of the maze." << std::endl << "You can go: ";

        if(current->north != NULL) {std::cout << "(N)orth, ";}
        if(current->south != NULL) {std::cout << "(S)outh, ";}
        if(current->east != NULL) {std::cout << "(E)ast, ";}
        if(current->west != NULL) {std::cout << "(W)est, ";}

        std::cout << "or (Q)uit" << std::endl;

        std::string s;
        std::cin >> s;
        std::cout << std::endl;

        if(s == "Q" || s == "q"){
            std::cout << "You have quit the maze." << std::endl << std::endl;
            return;
        }

        path += s;
        current = traverse_maze(start, path);
    }

    std::cout << "Congratulations, you reached the finish!" << std::endl << std::endl;
}

nodeptr random_walk(const nodeptr& start, const nodeptr& finish, std::string& path){
    srand(time(0));

    std::string arr[4] = {"N", "S", "E", "W"};
    nodeptr current = start;
    int i = 0;
    int index = 0;

    while((i < 20) && (current != finish)){
        while((current->north == NULL && index == 0) || (current->south == NULL && index == 1) || (current->east == NULL && index == 2) || (current->west == NULL && index == 3)){
            index = rand() % 4;
        }

        path += arr[index];
        current = traverse_maze(start, path);
        std::cout << current->id << " ";

        i++;
    }

    std::cout << std::endl;
    return current;
}

void solve_queue(const nodeptr& start, const nodeptr& finish, std::string& path){
    nodeptr current = start, prev = current;
    std::vector<nodeptr> visitednodes, tmp;
    int height = 1;

    if(bfs(start, finish, path, current, prev, visitednodes, tmp, height)){
        std::cout << path << std::endl;
        std::cout << "Congratulations, you reached the finish!" << std::endl << std::endl;
    }
}

bool bfs(const nodeptr& start, const nodeptr& finish, std::string& path, nodeptr& current, nodeptr& prev, std::vector<nodeptr>& visitednodes, std::vector<nodeptr>& tmp, int height){
    bool done = false, visit = false;
    bool checkNorth = true, checkSouth = true, checkEast = true, checkWest = true;
    int counter;

    if(current == finish){
        return true;
    }

    else{
        while(height != 0){
            counter = 0;
            visit = false;

            for(int i = 0; i < visitednodes.size(); i++){
                if(current->north == visitednodes[i]){
                    visit = true;
                }
            }

            for(int i = 0; i < tmp.size(); i++){
                if(current->north == tmp[i]){
                    visit = true;
                }
            }

            if(current->north != NULL && current->north != prev && visit == false){
                path += "N";
                tmp.push_back(current);
                counter++;

                for(int i = 0; i < visitednodes.size(); i++){
                    if(current->south == visitednodes[i] || current->south == NULL) {checkSouth = false;}
                    if(current->east == visitednodes[i] || current->east == NULL) {checkEast = false;}
                    if(current->west == visitednodes[i] || current->west == NULL) {checkWest = false;}
                }

                if(!checkSouth && !checkEast && !checkWest){
                    visitednodes.push_back(current);
                    done = bfs(current->north, finish, path, current->north, current->north, visitednodes, tmp, 1);
                }

                else{
                    done = bfs(start, finish, path, current->north, current, visitednodes, tmp, height-1);
                }

                checkSouth = true, checkEast = true, checkWest = true;

                if(done){
                    return true;
                }
            }

            visit = false;

            for(int i = 0; i < visitednodes.size(); i++){
                if(current->south == visitednodes[i]){
                    visit = true;
                }
            }

            for(int i = 0; i < tmp.size(); i++){
                if(current->south == tmp[i]){
                    visit = true;
                }
            }

            if(current->south != NULL && current->south != prev && visit == false){
                path += "S";
                tmp.push_back(current);
                counter++;

                for(int i = 0; i < visitednodes.size(); i++){
                    if(current->north == visitednodes[i] || current->north == NULL) {checkNorth = false;}
                    if(current->east == visitednodes[i] || current->east == NULL) {checkEast = false;}
                    if(current->west == visitednodes[i] || current->west == NULL) {checkWest = false;}
                }

                if(!checkNorth && !checkEast && !checkWest){
                    visitednodes.push_back(current);
                    done = bfs(current->south, finish, path, current->south, current->south, visitednodes, tmp, 1);
                }

                else{
                    done = bfs(start, finish, path, current->south, current, visitednodes, tmp, height-1);
                }

                checkNorth = true, checkEast = true, checkWest = true;

                if(done){
                    return true;
                }
            }

            visit = false;

            for(int i = 0; i < visitednodes.size(); i++){
                if(current->east == visitednodes[i]){
                    visit = true;
                }
            }

            for(int i = 0; i < tmp.size(); i++){
                if(current->east == tmp[i]){
                    visit = true;
                }
            }

            if(current->east != NULL && current->east != prev && visit == false){
                path += "E";
                tmp.push_back(current);
                counter++;

                for(int i = 0; i < visitednodes.size(); i++){
                    if(current->north == visitednodes[i] || current->north == NULL) {checkNorth = false;}
                    if(current->south == visitednodes[i] || current->south == NULL) {checkSouth = false;}
                    if(current->west == visitednodes[i] || current->west == NULL) {checkWest = false;}
                }

                if(!checkNorth && !checkSouth && !checkWest){
                    visitednodes.push_back(current);
                    done = bfs(current->east, finish, path, current->east, current->east, visitednodes, tmp, 1);
                }

                else{
                    done = bfs(start, finish, path, current->east, current, visitednodes, tmp, height-1);
                }

                checkNorth = true, checkSouth = true, checkWest = true;

                if(done){
                    return true;
                }
            }

            visit = false;

            for(int i = 0; i < visitednodes.size(); i++){
                if(current->west == visitednodes[i]){
                    visit = true;
                }
            }

            for(int i = 0; i < tmp.size(); i++){
                if(current->west == tmp[i]){
                    visit = true;
                }
            }

            if(current->west != NULL && current->west != prev && visit == false){
                path += "W";
                tmp.push_back(current);
                counter++;

                for(int i = 0; i < visitednodes.size(); i++){
                    if((current->north == visitednodes[i] || current->north == NULL)) {checkNorth = false;}
                    if(current->south == visitednodes[i] || current->south == NULL) {checkSouth = false;}
                    if(current->east == visitednodes[i] || current->east == NULL) {checkEast = false;}
                }

                if(!checkNorth && !checkSouth && !checkEast){
                    visitednodes.push_back(current);
                    done = bfs(current->west, finish, path, current->west, current->west, visitednodes, tmp, 1);
                }

                else{
                    done = bfs(start, finish, path, current->west, current, visitednodes, tmp, height-1);
                }

                checkNorth = true, checkSouth = true, checkEast = true;

                if(done){
                    return true;
                }
            }

            if(current == start) {
                height++;
            }

            else{
                height = 0;
            }

            for(int i = 0; i < counter; i++){
                tmp.pop_back();
            }
        }

        if(current->north == prev) {path += "N";}
        else if(current->south == prev) {path += "S";}
        else if(current->east == prev) {path += "E";}
        else if(current->west == prev) {path += "W";}

        if(visitednodes.size() == 0){
            if(current->north == NULL || current->north == prev) {checkNorth = false;}
            if(current->south == NULL || current->south == prev) {checkSouth = false;}
            if(current->east == NULL || current->east == prev) {checkEast = false;}
            if(current->west == NULL || current->west == prev) {checkWest = false;}
        }

        else{
            for(int i = 0; i < visitednodes.size(); i++){
                if(current->north == visitednodes[i] || current->north == NULL || current->north == prev) {checkNorth = false;}
                if(current->south == visitednodes[i] || current->south == NULL || current->south == prev) {checkSouth = false;}
                if(current->east == visitednodes[i] || current->east == NULL || current->east == prev) {checkEast = false;}
                if(current->west == visitednodes[i] || current->west == NULL || current->west == prev) {checkWest = false;}
            }
        }

        if(!checkNorth && !checkSouth && !checkEast && !checkWest){
            visitednodes.push_back(current);
        }
    }

    return false;
}

void solve_stack(const nodeptr& start, const nodeptr& finish, std::string& path){
    nodeptr current = start;
    std::stack<std::string> unvisited;
    std::stack<std::string> backtrack;
    std::vector<nodeptr> visited;

    visited.push_back(current);

    while(current != finish){
        bool checkNorth = true;
        bool checkSouth = true;
        bool checkEast = true;
        bool checkWest = true;

        for(int i = 0; i < visited.size(); i++){
            if (current->north == NULL || current->north == visited[i]) {checkNorth = false;}
            if (current->south == NULL || current->south == visited[i]) {checkSouth = false;}
            if (current->east == NULL || current->east == visited[i]) {checkEast = false;}
            if (current->west == NULL || current->west == visited[i]) {checkWest = false;}
        }

        if(!checkNorth && !checkSouth && !checkEast && !checkWest){
            path += backtrack.top();
            backtrack.pop();
        }

        else {
            if(checkWest) {unvisited.push("W");}
            if(checkEast) {unvisited.push("E");}
            if(checkSouth) {unvisited.push("S");}
            if(checkNorth) {unvisited.push("N");}

            path += unvisited.top();

            if(unvisited.top() == "N") {backtrack.push("S");}
            else if(unvisited.top() == "S") {backtrack.push("N");}
            else if(unvisited.top() == "E") {backtrack.push("W");}
            else if(unvisited.top() == "W") {backtrack.push("E");}

            unvisited.pop();
        }

        current = traverse_maze(start, path);
        std::cout << current->id << " ";
        visited.push_back(current);
    }

    std::cout << std::endl;
    std::cout << path << std::endl;
    std::cout << "Congratulations, you reached the finish!" << std::endl << std::endl;
}