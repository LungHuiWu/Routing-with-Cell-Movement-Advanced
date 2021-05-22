#include <string>
#include "Pin.h"

using namespace std;

Pin::Pin()
{
    Lyr = Layer();
}

Pin::Pin(string name, Layer layer)
{
    Name = name;
    Lyr = layer;
}

Pin::~Pin()
{
}