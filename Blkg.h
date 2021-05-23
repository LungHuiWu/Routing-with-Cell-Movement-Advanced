#include <string>
#include "Layer.h"

using namespace std;

class Blkg
{
private:
    string Name;
    string MC;
    Layer Lyr;
    int Demand;
public:
    Blkg();
    Blkg(string, Layer, int, string);
    ~Blkg();
};


