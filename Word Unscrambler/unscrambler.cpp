#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

void algo(string word, string s, vector<string>& out, vector<string>& v);
void alpha(string s, vector<string>& out, vector<string>& v, int start, int end);

struct alphabet {
    bool operator()(const string& a, const string& b) {
        return (a < b);
    }
};

int main(int argc, char **argv) {
    cout << "~ SCRABBLE UNSCRAMBLER ~" << endl << endl;

    string filename = "dictionary.txt";
    ifstream infile;
    infile.open(filename.c_str());

    if (!infile.is_open()) {
        cout << "Dictionary unavailable" << endl;
        cout << "Please install a working dictionary and try again" << endl << endl;
        exit(EXIT_FAILURE);
    }

    string word;
    cout << "Enter letters to unscramble:" << endl;
    cin >> word;
    cout << endl;

    cout << "Loading..." << endl << endl;

    for (int i = 0; i < word.length(); i++) {
        if ((word[i] >= 65 && word[i] <= 90) || (word[i] >= 97 && word[i] <= 122)) {
            if (word[i] >= 97 && word[i] <= 122) {
                word[i] -= 32;
            }
        }

        else {
            cout << "Invalid input!" << endl << endl;
            exit(EXIT_FAILURE);
        }
    }

    vector<string> v, out;
    string dict;

    while (infile >> dict) {
        v.push_back(dict);
    }

    algo(word, "", out, v);

    alphabet a;
    sort(out.begin(), out.end(), a);

    system("clear");
    cout << "The words generated from " << word << " are:" << endl << endl;

    for (int i = 2; i <= word.length(); i++) {
        cout << i << " letter words:" << endl;

        for (int j = 0; j < out.size(); j++) {
            if (out[j].length() == i) {
                cout << out[j] << endl;
            }
        }

        cout << endl;
    }

    cout << endl;
    return 0;
}

void algo(string word, string s, vector<string>& out, vector<string>& v) {
    int start, end;

    if (s.length() >= 2) {
        switch (s[0]) {
            case 'A': start = 0;
                    end = 16082;
                    break;
            case 'B': start = 16083;
                    end = 31130;
                    break;
            case 'C': start = 31131;
                    end = 55913;
                    break;
            case 'D': start = 55914;
                    end = 72421;
                    break;
            case 'E': start = 72422;
                    end = 83691;
                    break;
            case 'F': start = 83692;
                    end = 94198;
                    break;
            case 'G': start = 94199;
                    end = 103475;
                    break;
            case 'H': start = 103476;
                    end = 113901;
                    break;
            case 'I': start = 113902;
                    end = 123447;
                    break;
            case 'J': start = 123448;
                    end = 125722;
                    break;
            case 'K': start = 125723;
                    end = 129082;
                    break;
            case 'L': start = 129083;
                    end = 137057;
                    break;
            case 'M': start = 137058;
                    end = 152689;
                    break;
            case 'N': start = 152690;
                    end = 159145;
                    break;
            case 'O': start = 159146;
                    end = 167954;
                    break;
            case 'P': start = 167955;
                    end = 192059;
                    break;
            case 'Q': start = 192060;
                    end = 193458;
                    break;
            case 'R': start = 193459;
                    end = 208341;
                    break;
            case 'S': start = 208342;
                    end = 239986;
                    break;
            case 'T': start = 239987;
                    end = 254359;
                    break;
            case 'U': start = 254360;
                    end = 263743;
                    break;
            case 'V': start = 263744;
                    end = 268282;
                    break;
            case 'W': start = 268283;
                    end = 274139;
                    break;
            case 'X': start = 274140;
                    end = 274448;
                    break;
            case 'Y': start = 274449;
                    end = 275479;
                    break;
            case 'Z': start = 275480;
                    end = 276642;
                    break;
        }

        alpha(s, out, v, start, end);
    }

    for (int i = 0; i < word.length(); i++) {
        algo(string(word).erase(i, 1), s + word[i], out, v);
    }
}

void alpha(string s, vector<string>& out, vector<string>& v, int start, int end) {
    for (int i = start; i < end; i++) {
        if (s == v[i]) {
            out.push_back(s);
            v.erase(v.begin() + i);
            return;
        }
    }
}