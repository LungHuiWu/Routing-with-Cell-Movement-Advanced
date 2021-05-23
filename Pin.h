#include <string>
#include "Layer.h"

using namespace std;

class Pin
{
private:
    string Name;
    string MC;
    Layer Lyr;
    int Row;
    int Col;
    bool Connected;
public:
    Pin();
    Pin(string, Layer, string);
    ~Pin();
    void setMC(string);
    void Connect();
    void Disconnect();
    string getName();
};


