#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>

using namespace std;

class TuringMachine
{
    vector<string> strip;
    string statement = "start";
    int cursor;

    inline bool test_for_primary_cursor(string cell)
    {
        return cell[0] == '|' && cell[cell.size() - 1] == '|';
    }

public:
    bool is_end(string dir);
    bool load_strip(string dir);
    string get_current_state();
    string get_current_word();
    string get_strip();
    void set_current_state(string a);
    void set_current_word(string a);
    bool get_step(char a);
};
