#ifndef PIN_H
#define PIN_H
#include <string>
#include "Layer.h"

using namespace std;

class Pin
{
private:
    string Name;
    string Net;
    string MC;
    Layer Lyr;
    int Row;
    int Col;
    bool Connected; // True if it is connected by a net
public:
    Pin();
    Pin(string, Layer, string);
    ~Pin();
    string getNetname();
    void setMC(string);
    void Connect(string);
    void Disconnect();
    string getName();
    
};

#endif // PIN_H
