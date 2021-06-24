#include <string>
#include <iostream>
#include "GGrid.h"

using namespace std;

GGrid::GGrid()
{
    rowIdx = 0;
    colIdx = 0;
    LyrIdx = 0;
}

GGrid::GGrid(int row, int col, Layer* layer)
{
    rowIdx = row;
    colIdx = col;
    Lyr = layer;
    LyrIdx = Lyr->getIdx();
    supply = Lyr->getsupply();
    NetLink.clear();
}

GGrid::~GGrid()
{
}

void GGrid::adjustSupply(int i)
{
    supply += i;
}

int GGrid::getSupply()
{
    return supply;
}

void GGrid::addNet(string s)
{
    NetLink[s] = false;
}

void GGrid::linkNet(string s)
{
    if (NetLink[s]==false)
    {
        supply -= 1;
    }
    NetLink[s] = true;
}