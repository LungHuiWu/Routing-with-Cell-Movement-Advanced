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
public:
    Design(/* args */);
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
};
