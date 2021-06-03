#ifndef NET_H
#define NET_H
#include <string>
#include <vector>
#include "GGrid.h"
#include "Route.h"
#include "CellInst.h"

using namespace std;

class Net
{
private:
    string Name;
    int NumPin;
    int MinLayer;
    double Weight;
    vector<Route*> RList;
    vector<string> conCIs; //CIs connected to the Net
public:
    Net();
    Net(string, int, int, double);
    ~Net();
    void connect(CellInst, string); // For connecting nets onto pins
    void addRoute(int, int, int, int, int, int);
    Net& operator = (const Net&);
    string getName();
    double getWeight();
    vector<string> getCIs();
};

#endif // NET_H