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
    NetLink[s] = 0;
}

void GGrid::linkNet(string s)
{
    if (NetLink[s]==0)
    {
        supply -= 1;
    }
    NetLink[s] += 1;
}

void GGrid::unlinkNet(string s)
{
    if (NetLink[s] == 1)
    {
        supply += 1;
    }
    if (NetLink[s]<0)
    {
        cout << "???" << endl;
    }
    NetLink[s] -= 1;
}