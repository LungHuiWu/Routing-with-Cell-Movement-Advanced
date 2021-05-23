#include <string>
#include <iostream>
#include "CellInst.h"

using namespace std;

CellInst::CellInst()
{
    Row = 0;
    Col = 0;
    Movable = false;
}

CellInst::CellInst(string instname, string mcname, int row, int col, bool mov)
{
    CellName = instname;
    MCName = mcname;
    Row = row;
    Col = col;
    Movable = mov;
}

CellInst::~CellInst()
{
}
