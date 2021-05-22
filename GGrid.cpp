#include <string>
#include <iostream>
#include "GGrid.h"

using namespace std;

GGrid::GGrid()
{
    rowIdx = 0;
    colIdx = 0;
    Lyr = Layer();
}

GGrid::GGrid(int row, int col, Layer layer)
{
    rowIdx = row;
    colIdx = col;
    Lyr = layer;
    supply = Lyr.getsupply();
}

GGrid::~GGrid()
{
}