#include <string>
#include "GGrid.h"
#include "Pin.h"
#include "MCell.h"
#include "Route.h"

using namespace std;

class Net
{
private:
    string Name;
    int NumPin;
    int MinLayer;
    int Weight;
    Route* route;
public:
    Net();
    Net(string, int, int, int);
    ~Net();
};

