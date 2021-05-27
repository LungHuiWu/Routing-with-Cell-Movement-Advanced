#ifndef GGRID_H
#define GGRID_H
#include <string>
#include "Layer.h"

using namespace std;

class GGrid
{
private:
    int rowIdx, colIdx, LyrIdx;
    Layer* Lyr;
    int supply;
public:
    GGrid();
    GGrid(int, int, Layer*);
    ~GGrid();
    void adjustSupply(int); // For non-default-supply-ggrids
};

#endif // GGRID_H
