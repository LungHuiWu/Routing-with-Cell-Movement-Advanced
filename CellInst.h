#ifndef CELLINST_H
#define CELLINST_H
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include "Pin.h"
#include "Blkg.h"
#include "MCell.h"
#include "Net.h"

using namespace std;

class CellInst
{
private:
    string InstName;
    string MCName;
    bool Movable;
    int Row;
    int Col;
    // Temporarily implement using vector, linked list(pointer) can used to save memory and time.
    vector<Pin*> PList;
    vector<Blkg*> BList;
    vector<tuple<int,int>> VtgArea; // There are better data structures
    int VtgAreaGGridCount;
    vector<string> CIs;

public:
    CellInst();
    CellInst(MCell, string, int, int, bool);
    ~CellInst();
    vector<Pin*> getPList();
    vector<Blkg*> getBList();
    void setVtgArea(int, int);
    string getCIName();
    bool hasVtgArea();
    vector<tuple<int,int>> getVtgArea();
    tuple<int,int> getLocation();
    vector<string> getADJCIs(map<string,Net>);
    vector<string> getADJNets();
    void Relocate(tuple<int,int>&);
};

#endif // CELLINST_H