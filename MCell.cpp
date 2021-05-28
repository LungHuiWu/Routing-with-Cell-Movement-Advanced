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
    Pin a = Pin(name, l, MCName);
    PList.push_back(&a);
    cout << PList.size() << endl;
}

void MCell::addBlkg(string name, Layer l, double d)
{
    Blkg a = Blkg(name, l, d, MCName);
    BList.push_back(&a);
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