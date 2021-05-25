#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include "Layer.h"
#include "Blkg.h"
#include "CellInst.h"
#include "Design.h"
#include "GGrid.h"
#include "MCell.h"
#include "Net.h"
#include "Pin.h"
#include "Route.h"

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
        
    }
}