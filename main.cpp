#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include "Design.h"
//hi
using namespace std;

int main(int argc, char** argv)
{
    // TODO
    // Parse the command and generate a design
    // argv[1]: input file name, argv[2]: output file name
    if (argc != 3)
    {
        cout << "Usage: ./cell_move_router <input.txt> <output.txt>";
    }
    else
    {
        Design D = Design();
        D.readFile(argv[1]);
        string s = D.select();
        cout << "The selected cell is " << s << "." << endl;
        vector<tuple<int, int>> c = D.placement(s);
        for (int i = 0; i<2; i++)
        {
            if (i==0)
            {
                cout << "The first new place of " << s << " is (" << get<0>(c[i]) << "," << get<1>(c[i]) << ")." << endl;
            }
            else if (i==1)
            {
                cout << "The second new place of " << s << " is (" << get<0>(c[i]) << "," << get<1>(c[i]) << ")." << endl;
            }   
        }
        D.routing(s,c[0]);
    }
    return 1;
}