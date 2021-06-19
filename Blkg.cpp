#include <string>
#include "Blkg.h"

using namespace std;

Blkg::Blkg()
{
    Demand = 0;
    Lyr = Layer();
}

Blkg::Blkg(string name, Layer layer, double d, string mc)
{
    Name = name;
    Lyr = layer;
    Demand = d;
    MC = mc;
}

Blkg::~Blkg()
{
}

Layer Blkg::getLayer()
{
    return Lyr;
}

double Blkg::getDemand()
{
    return Demand;
}

string Blkg::getName()
{
    return Name;
}