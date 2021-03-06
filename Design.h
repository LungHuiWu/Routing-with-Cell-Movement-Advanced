#ifndef DESIGN_H
#define DESIGN_H
#include <string>
#include <vector>
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
    map<int,Layer*> LList_idx;
    GGrid*** GGridList;
    map<string,MCell> MCList;
    map<string,CellInst> CIList;
    map<string,CellInst> mCIList; //the Cells to be replaced
    map<string,Net> NList;
    vector<string> ADJCIs;
    vector<string> adjNets;
    vector<Route*> delRList;
    vector<string> selected;
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
    int getMax();
    map<string,Net> getNList();
    string select();
    vector<tuple<int,int>> placement(string&);
    double routing(string&, tuple<int, int>,int);
    double calculate(vector<Route*>, double);
    void addRoute(int, int, int, int, int, int, string);
    void delRoute(Route*, string);
    void clearGGridstep();
    void clearGGridCovered();
    void clearAncestor();
    void setGGridCovered(Route*,int);
    void setCICovered(vector<Route*>&,int,int,int,int);
    void showCovered();
    void showColor();
    void showStep();
    vector<Route*> mst(int,int,int,int,int,string);
    void setWhite();
};

#endif // DESIGN_H