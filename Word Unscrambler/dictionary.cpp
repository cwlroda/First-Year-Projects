#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

int main() {
    string filename = "dictionary.txt";
    ifstream infile;
    infile.open(filename.c_str());

    vector<string> a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
    string dict;
    int counter = 0;

    while (infile >> dict) {
        counter++;

        switch (dict[0]) {
            case 'A': a.push_back(dict);
                    //break;
            case 'B': b.push_back(dict);
                    //break;
            case 'C': c.push_back(dict);
                    //break;
            case 'D': d.push_back(dict);
                    //break;
            case 'E': e.push_back(dict);
                    //break;
            case 'F': f.push_back(dict);
                    //break;
            case 'G': g.push_back(dict);
                    //break;
            case 'H': h.push_back(dict);
                    //break;
            case 'I': i.push_back(dict);
                    //break;
            case 'J': j.push_back(dict);
                    //break;
            case 'K': k.push_back(dict);
                    //break;
            case 'L': l.push_back(dict);
                    //break;
            case 'M': m.push_back(dict);
                    //break;
            case 'N': n.push_back(dict);
                    //break;
            case 'O': o.push_back(dict);
                    //break;
            case 'P': p.push_back(dict);
                    //break;
            case 'Q': q.push_back(dict);
                    //break;
            case 'R': r.push_back(dict);
                    //break;
            case 'S': s.push_back(dict);
                    //break;
            case 'T': t.push_back(dict);
                    //break;
            case 'U': u.push_back(dict);
                    //break;
            case 'V': v.push_back(dict);
                    //break;
            case 'W': w.push_back(dict);
                    //break;
            case 'X': x.push_back(dict);
                    //break;
            case 'Y': y.push_back(dict);
                    //break;
            case 'Z': z.push_back(dict);
                    //break;
        }
    }

    cout << "A has " << a.size() << " words" << endl;
    cout << "B has " << b.size() << " words" << endl;
    cout << "C has " << c.size() << " words" << endl;
    cout << "D has " << d.size() << " words" << endl;
    cout << "E has " << e.size() << " words" << endl;
    cout << "F has " << f.size() << " words" << endl;
    cout << "G has " << g.size() << " words" << endl;
    cout << "H has " << h.size() << " words" << endl;
    cout << "I has " << i.size() << " words" << endl;
    cout << "J has " << j.size() << " words" << endl;
    cout << "K has " << k.size() << " words" << endl;
    cout << "L has " << l.size() << " words" << endl;
    cout << "M has " << m.size() << " words" << endl;
    cout << "N has " << n.size() << " words" << endl;
    cout << "O has " << o.size() << " words" << endl;
    cout << "P has " << p.size() << " words" << endl;
    cout << "Q has " << q.size() << " words" << endl;
    cout << "R has " << r.size() << " words" << endl;
    cout << "S has " << s.size() << " words" << endl;
    cout << "T has " << t.size() << " words" << endl;
    cout << "U has " << u.size() << " words" << endl;
    cout << "V has " << v.size() << " words" << endl;
    cout << "W has " << w.size() << " words" << endl;
    cout << "X has " << x.size() << " words" << endl;
    cout << "Y has " << y.size() << " words" << endl;
    cout << "Z has " << z.size() << " words" << endl << endl;

    cout << "Total number of words: " << counter << endl << endl;

    return 0;
}