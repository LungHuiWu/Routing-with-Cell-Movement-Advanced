#ifndef NET_H
#define NET_H
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "GGrid.h"
#include "Route.h"

using namespace std;

class CellInst;

class Net
{
private:
    string Name;
    int NumPin;
    int MinLayer;
    double Weight;
    vector<Route*> RList;
    vector<string> conCIs; //CIs connected to the Net
    vector<Route*> R;
public:
    Net();
    Net(string, int, int, double);
    ~Net();
    void connect(CellInst, string); // For connecting nets onto pins
    void Disconnect(CellInst, string);
    void addRoute(int, int, int, int, int, int);
    Net& operator = (const Net&);
    string getName();
    double getWeight();
    vector<string> getCIs();
    void delRoute(CellInst, tuple<int, int, int>, map<string,CellInst>);
    vector<Route*> getRList();
    vector<string> delCI; //CIs which is disconnected during delroute
    vector<Route*> getR();
    int getMinLyr();
    void addtoRList(vector<Route*>);
    void clearR();
};

#endif // NET_H