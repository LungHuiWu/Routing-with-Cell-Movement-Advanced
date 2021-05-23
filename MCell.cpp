#include <string>
#include <iostream>
#include "MCell.h"

using namespace std;

MCell::MCell()
{
    NumBlkg = 0;
    NumPin = 0;
}

MCell::MCell(string name, int numpin, int numblkg)
{
    Name = name;
    NumPin = numpin;
    NumBlkg = numblkg;
}

MCell::~MCell()
{
}