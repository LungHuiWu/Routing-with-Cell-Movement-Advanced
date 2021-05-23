#include <string>
#include "GGrid.h"
#include "Pin.h"
#include "Blkg.h"
#include "MCell.h"

using namespace std;

class CellInst
{
private:
    string CellName;
    string MCName;
    bool Movable;
    int Row;
    int Col;
    GGrid* Grid;

public:
    CellInst();
    CellInst(string, string, int, int, bool);
    ~CellInst();
};
