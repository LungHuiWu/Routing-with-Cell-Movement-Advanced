#include <string>
#include "Layer.h"

using namespace std;

class GGrid
{
private:
    int rowIdx, colIdx;
    Layer Lyr;
    int supply;
public:
    GGrid();
    GGrid(int, int, Layer);
    ~GGrid();
};


