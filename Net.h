#include <string>
#include <vector>
#include "GGrid.h"
#include "Pin.h"
#include "MCell.h"
#include "Route.h"
#include "CellInst.h"

using namespace std;

class Net
{
private:
    string Name;
    int NumPin;
    int MinLayer;
    int Weight;
    vector<Route> RList;
public:
    Net();
    Net(string, int, int, int);
    ~Net();
    void connect(CellInst, string);
    void addRoute(int, int, int, int, int, int);
};

