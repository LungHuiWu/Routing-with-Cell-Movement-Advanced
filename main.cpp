#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include "Design.h"

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
    }
    return 1;
}