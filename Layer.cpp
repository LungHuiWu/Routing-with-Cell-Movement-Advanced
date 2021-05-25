#include <string>
#include <iostream>
#include "Layer.h"

using namespace std;

Layer::Layer()
{
    Idx = 0;
    DefaultSupply = 0;
}

Layer::Layer(string name, int index, char dir, int dsupply, double pf)
{
    Name = name;
    Idx = index;
    Dir = dir;
    DefaultSupply = dsupply;
    Powerfactor = pf;
}

Layer::~Layer(){
}

int Layer::getsupply(){
    return DefaultSupply;
}

int Layer::getIdx(){
    return Idx;
}

Layer& Layer::operator = (const Layer& l)
{
    Name = l.Name; //newnew
    Idx = l.Idx;
    Dir = l.Dir;
    DefaultSupply = l.DefaultSupply;
    Powerfactor = l.Powerfactor;
    return *this;
}

bool Layer::operator == (const Layer& l)
{
    return (this->Idx)==l.Idx;
}

bool Layer::operator > (const Layer& l)
{
    return (this->Idx)>l.Idx;
}

bool Layer::operator < (const Layer& l)
{
    return (this->Idx)<l.Idx;
}

bool Layer::operator <= (const Layer& l)
{
    return (this->Idx)<=l.Idx;
}

bool Layer::operator >= (const Layer& l)
{
    return (this->Idx)>=l.Idx;
}

bool Layer::LowerError(int l)
{
    return Idx<l;
}