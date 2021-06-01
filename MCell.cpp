#include <string>
#include <iostream>
#include <vector>
#include "MCell.h"

using namespace std;

MCell::MCell()
{
    NumBlkg = 0;
    NumPin = 0;
}

MCell::MCell(string name, int numpin, int numblkg)
{
    MCName = name;
    NumPin = numpin;
    NumBlkg = numblkg;
}

MCell::~MCell()
{
}

string MCell::getName()
{
    return MCName;
}

vector<Pin*> MCell::getPList()
{
    return PList;
}

vector<Blkg*> MCell::getBList()
{
    return BList;
}

void MCell::addPin(string name, Layer l)
{
    PList.push_back(new Pin(name, l, MCName));
}

void MCell::addBlkg(string name, Layer l, double d)
{
    BList.push_back(new Blkg(name, l, d, MCName));
}

MCell& MCell::operator = (const MCell& m)
{
    MCName = m.MCName;
    NumBlkg = m.NumBlkg;
    NumPin = m.NumPin;
    PList = m.PList;
    BList = m.BList;
    return *this;
}