#include <string>
#include <iostream>
#include "Layer.h"

using namespace std;

Layer::Layer()
{
    Idx = 0;
    DefaultSupply = 0;
}

Layer::Layer(string name, int index, char dir, int dsupply, int pf)
{
    Name = name;
    Idx = index;
    Dir = dir;
    DefaultSupply = dsupply;
    Powerfactor = pf;
}

Layer::~Layer(){
}