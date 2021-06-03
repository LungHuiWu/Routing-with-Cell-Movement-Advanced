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
    Net = "";
}

Pin::~Pin()
{
}

void Pin::setMC(string n)
{
    MC = n;
}

void Pin::Connect(string n)
{
    Connected = true;
    Net = n;
}

void Pin::Disconnect()
{
    Connected = false;
    Net = "";
}

string Pin::getName()
{
    return Name;
}

string Pin::getNetname()
{
    return Net;
}

