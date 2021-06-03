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

CellInst::CellInst(MCell mc, string instname, int row, int col, bool mov)
{
    InstName = instname;
    MCName = mc.getName();
    Row = row;
    Col = col;
    PList = mc.getPList();
    BList = mc.getBList();
    Movable = mov;
    VtgAreaGGridCount = 0;
}

CellInst::~CellInst()
{
}

vector<Pin*> CellInst::getPList()
{
    return PList;
}

void CellInst::setVtgArea(int x, int y)
{
    VtgArea.push_back(make_tuple(x,y));
    VtgAreaGGridCount += 1;
}

string CellInst::getCIName()
{
    return InstName;
}

vector<tuple<int,int>> CellInst::getVtgArea()
{
    return VtgArea;
}

tuple<int,int> CellInst::getLocation()
{
    return make_tuple(Row,Col);
}