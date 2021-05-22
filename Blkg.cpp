#include <string>
#include "Blkg.h"

using namespace std;

Blkg::Blkg()
{
    Demand = 0;
    Lyr = Layer();
}

Blkg::Blkg(string name, Layer layer, int d)
{
    Name = name;
    Lyr = layer;
    Demand = d;
}

Blkg::~Blkg()
{
}