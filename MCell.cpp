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
    return Name;
}

vector<Pin> MCell::getPList()
{
    return Plist;
}

vector<Blkg> MCell::getBList()
{
    return Blist;
}

void MCell::addPin(string name, Layer l)
{
    PList.push_back(Pin(name, l, MCName));
}

void MCell::addBlkg(string name, Layer l, int d)
{
    BList.push_back(Blkg(name, l, d, MCName));
}