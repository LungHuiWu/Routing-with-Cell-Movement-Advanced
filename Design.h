#include <string>
#include "GGrid.h"
#include "Pin.h"
#include "MCell.h"
#include "Route.h"

using namespace std;

class Design
{
private:
    int MaxCellMove;
    int RowMin, RowMax;
    int ColMin, ColMax;
    int NumLyr, NumNonDefaultSupplyGGrid, NumMCell, NumCellInst, NumNet, NumVtgArea, NumRoute;
    vector<Layer*> LList;
public:
    Design();
    ~Design();
    void setMaxCellMove(int);
    void setBoundary(int, int, int, int);
    void setNumLyr(int);
    void setNumNonDefaultSupplyGGrid(int);
    void setNumMCell(int);
    void setNumCellInst(int);
    void setNumNet(int);
    void setNumVtgArea(int);
    void setNumRoute(int);
    // Read file //
    bool readFile(const string&);
    void readMaxCellMove(string&);
    void readGGridBoundaryIdx(string&);
    void readLayer(string&);
    void readNonDefaultGGrid(string&);
    void readMCell(string&);
    void readCellInst(string&);
    void readNet(string&);
    void readVtgArea(string&);
    void readRoute(string&);
    // Algorithms //
};
