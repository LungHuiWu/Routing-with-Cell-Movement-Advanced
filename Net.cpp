#include <string>
#include <iostream>
#include "Net.h"

using namespace std;

Net::Net()
{
    NumPin = 0;
    MinLayer = 0;
    Weight = 0;
}

Net::Net(string name, int num, int min, int w)
{
    Name = name;
    NumPin = num;
    MinLayer = min;
    Weight = w;
}

Net::~Net()
{
}
