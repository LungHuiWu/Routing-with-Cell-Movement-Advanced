#ifndef BLKG_H
#define BLKG_H
#include <string>
#include "Layer.h"

using namespace std;

class Blkg
{
private:
    string Name;
    string MC;
    Layer Lyr;
    double Demand;
public:
    Blkg();
    Blkg(string, Layer, double, string);
    ~Blkg();
};

#endif // BLKG_H
