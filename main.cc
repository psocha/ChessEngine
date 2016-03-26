#include <iostream>
#include <string>
using namespace std;

int main() {
    string line;
    cout.setf(ios::unitbuf);

    while (getline(cin, line)) {
        if (line == "uci") {
            cout << "id name random" << endl;
            cout << "id author psocha" << endl;
        } else if (line == "isready") {
            cout << "readyok" << endl;
        } else if (line.substr(0, 9) == "position ") {

        } else if (line.substr(0, 3) == "go ") {
            cout << "bestmove d7d5" << endl;
        } else if (line == "quit") {
            break;
        }
    }

    return 0;
}

