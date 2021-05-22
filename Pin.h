#include <string>
#include "Layer.h"

using namespace std;

class Pin
{
private:
    string Name;
    Layer Lyr;
    int row;
    int col;
public:
    Pin();
    Pin(string, Layer);
    ~Pin();
};


