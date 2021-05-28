#include <string>
#include <iostream>
#include "Pin.h"

using namespace std;

Pin::Pin()
{
    Lyr = Layer();
}

Pin::Pin(string name, Layer layer, string mc)
{
    Name = name;
    Lyr = layer;
    MC = mc;
    Connected = false;
}

Pin::~Pin()
{
}

void Pin::setMC(string n)
{
    MC = n;
}

void Pin::Connect()
{
    Connected = true;
}

void Pin::Disconnect()
{
    Connected = false;
}

string Pin::getName()
{
    return Name;
}