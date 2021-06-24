#ifndef GGRID_H
#define GGRID_H
#include <string>
#include <vector>
#include <map>
#include "Layer.h"

using namespace std;

class GGrid
{
private:
    int rowIdx, colIdx, LyrIdx;
    Layer* Lyr;
    int supply;
    map <string,bool> NetLink;
public:
    GGrid();
    GGrid(int, int, Layer*);
    ~GGrid();
    void adjustSupply(int); // For non-default-supply-ggrids
    int getSupply();
    void addNet(string);
    void linkNet(string);
};

#endif // GGRID_H
