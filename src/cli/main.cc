/* cli/main.cc - Commandline interface to CARVE
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.hh"

using namespace carve;

int main(int argc, char** argv) {

    if (argc != 2) {
        cerr << "Usage: carve [file]" << endl;
        return 1;
    }

    string fname = argv[1];
    ifstream ifs(fname, ios::in);
    if (!ifs.is_open()) {
        cerr << "Unknown file: " << fname << endl;
        return 1;
    }
    string src = {istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};
    ifs.close();


    // Tokenize the input
    vector<Token> toks = lex(fname, src);
    if (toks.size() == 0) {
        return 1;
    }

    // Parse 
    Program* p = parse(fname, src, toks, true);
    if (!p) {
        return 1;
    }

    State* s = new State();
    
    // Initialize from the program
    s->init(*p);

    // Execute until it becomes halted
    while (!s->is_halted) {
        carve_exec_single(s);
    }

    for (int i = 0; i < NUM_RX; ++i) {
        cout << "x" << i << ": " << s->rx[i] << endl;
    }

    // Clean up
    delete s;
    delete p;

}

