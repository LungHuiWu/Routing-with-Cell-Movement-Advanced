#include <string>
#include <iostream>
#include "Design.h"

using namespace std;

Design::Design()
{
}

Design::~Design()
{
}

void Design::setMaxCellMove(int max)
{
    MaxCellMove = max;
}

void Design::setBoundary(int a, int b, int c, int d)
{
    RowMin = a;
    ColMin = b;
    RowMax = c;
    ColMax = d;
}

void Design::setNumLyr(int l)
{
    NumLyr = l;
}

void Design::setNumNonDefaultSupplyGGrid(int n)
{
    NumNonDefaultSupplyGGrid = n;
}

void Design::setNumMCell(int mc)
{
    NumMCell = mc;
}

void Design::setNumCellInst(int cell)
{
    NumCellInst = cell;
}

void Design::setNumNet(int  n)
{
    NumNet = n;
}

void Design::setNumVtgArea(int va)
{
    NumVtgArea = va;
}

void Design::setNumRoute(int r)
{
    NumRoute = r;
}
