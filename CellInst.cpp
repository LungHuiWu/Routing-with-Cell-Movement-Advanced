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

bool CellInst::hasVtgArea()
{
    return (VtgAreaGGridCount!=0);
}

vector<string> CellInst::getADJCIs(map<string,Net> NList) //find CIs connected to the CI
{   
    vector<string> c;
    for(int i = 0; i<getPList().size(); i++)
    {
        string net = getPList()[i]->getNetname();
        if(net != "")
        {
            CIs = NList[net].getCIs();
            for(int i = 0; i<CIs.size(); i++)
            {
                if (find(c.begin(), c.end(), CIs[i]) == c.end())
                {
                    c.push_back(CIs[i]);
                }
            }
        }
    }
    return c;
}

vector<string> CellInst::getADJNets() //find CIs connected to the CI
{   
    vector<string> n;
    for(int i = 0; i<getPList().size(); i++)
    {
        string net = getPList()[i]->getNetname();
        if(net != "")
        {
           if (find(n.begin(), n.end(), net) == n.end()) n.push_back(net);
        }
    }
    return n;
}