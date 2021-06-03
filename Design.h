#ifndef DESIGN_H
#define DESIGN_H
#include <string>
#include <vector>
#include <map>
#include "GGrid.h"
#include "Pin.h"
#include "MCell.h"
#include "Route.h"
#include "CellInst.h"
#include "Net.h"

using namespace std;

class Design
{
private:
    int MaxCellMove;
    int RowMin, RowMax;
    int ColMin, ColMax;
    int NumRow, NumCol;
    int NumLyr, NumNonDefaultSupplyGGrid, NumMCell, NumCellInst, NumNet, NumVtgArea, NumRoute;
    map<string,Layer*> LList;
    GGrid*** GGridList;
    map<string,MCell> MCList;
    map<string,CellInst> CIList;
    map<string,CellInst> mCIList; //the Cells to be replaced
    map<string,Net> NList;
public:
    Design();
    ~Design();
    // Read file //
    bool readFile(const string&);
    void readMaxCellMove(string&);
    void readGGridBoundaryIdx(string&);
    void readLayer(string&);
    void readNonDefaultGGrid(string&);
    string readMCell(string&, string&);
    void readCellInst(string&);
    string readNet(string&, string&);
    void readVtgArea(string&);
    void readVtgArea2(string&, string&);
    void readRoute(string&);
    // Algorithms //
    string select();
    vector<tuple<int,int>> placement(string&);
    void routing();
};

#endif // DESIGN_H