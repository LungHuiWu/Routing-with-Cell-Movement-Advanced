#include <string>
#include <vector>
#include <tuple>
#include "GGrid.h"
#include "Pin.h"
#include "Blkg.h"
#include "MCell.h"

using namespace std;

class CellInst
{
private:
    string InstName;
    string MCName;
    bool Movable;
    int Row;
    int Col;
    GGrid* Grid;
    // Temporarily implement using vector, linked list(pointer) can used to save memory and time.
    vector<Pin*> PList;
    vector<Blkg*> BList;
    vector<tuple<int,int>> VtgArea; // There are better data structures
    int VtgAreaGGridCount;

public:
    CellInst();
    CellInst(MCell, string, int, int, bool);
    ~CellInst();
    vector<Pin*> getPList();
    void setVtgArea(int, int);
};
