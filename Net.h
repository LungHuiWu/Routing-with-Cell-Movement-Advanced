#include <string>
#include "GGrid.h"
#include "Pin.h"
#include "MCell.h"

using namespace std;

class Net
{
private:
    string Name;
    int NumPin;
    int MinLayer;
    int Weight;
public:
    Net();
    Net(string, int, int, int);
    ~Net();
};

