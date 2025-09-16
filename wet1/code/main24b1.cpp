// 
// 234218 Data Structures 1.
// Semester: 2024B (spring).
// Wet Exercise #1.
// 
// The following main file is necessary to link and run your code.
// This file is READ ONLY: even if you submit something else, the compiler ..
// .. WILL use our file.
// 

#include "pirates24b1.h"
#include <string>
#include <iostream>

using namespace std;

void print(string cmd, StatusType res);
void print(string cmd, output_t<int> res);

int main()
{
    
    int d1, d2, d3;

    // Init
    Ocean *obj = new Ocean();
    
    // Execute all commands in file
    string op;
    while (cin >> op)
    {
        if (!op.compare("add_ship")) {
            cin >> d1 >> d2;
            print(op, obj->add_ship(d1, d2));
        } else if (!op.compare("remove_ship")) {
            cin >> d1;
            print(op, obj->remove_ship(d1));
        } else if (!op.compare("add_pirate")) {
            cin >> d1 >> d2 >> d3;
            print(op, obj->add_pirate(d1, d2, d3));
        } else if (!op.compare("remove_pirate")) {
            cin >> d1;
            print(op, obj->remove_pirate(d1));
        } else if (!op.compare("treason")) {
            cin >> d1 >> d2;
            print(op, obj->treason(d1, d2));
        } else if (!op.compare("update_pirate_treasure")) {
            cin >> d1 >> d2;
            print(op, obj->update_pirate_treasure(d1, d2));
        } else if (!op.compare("get_treasure")) {
            cin >> d1;
            print(op, obj->get_treasure(d1));
        } else if (!op.compare("get_cannons")) {
            cin >> d1;
            print(op, obj->get_cannons(d1));
        } else if (!op.compare("get_richest_pirate")) {
            cin >> d1;
            print(op, obj->get_richest_pirate(d1));
        } else if (!op.compare("ships_battle")) {
            cin >> d1 >> d2;
            print(op, obj->ships_battle(d1, d2));
        } else {
            cout << "Unknown command: " << op << endl;
            return -1;
        }
        // Verify no faults
        if (cin.fail()){
            cout << "Invalid input format" << endl;
            return -1;
        }
    }

    // Quit 
    delete obj;
    return 0;
}

// Helpers
static const char *StatusTypeStr[] =
{
    "SUCCESS",
    "ALLOCATION_ERROR",
    "INVALID_INPUT",
    "FAILURE"
};

void print(string cmd, StatusType res) 
{
    cout << cmd << ": " << StatusTypeStr[(int) res] << endl;
}

void print(string cmd, output_t<int> res)
{
    if (res.status() == StatusType::SUCCESS) {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << ", " << res.ans() << endl;
    } else {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
    }
}
