#ifndef LAYER_H
#define LAYER_H
#include <string>

using namespace std;

class Layer
{
private:
    /* data */
    string Name;
    int Idx;
    char Dir;
    int DefaultSupply;
    double Powerfactor;

public:
    Layer();
    Layer(string, int, char, int, double);
    virtual ~Layer();
    int getsupply();
    int getIdx();
    Layer& operator = (const Layer&);
    bool operator == (const Layer&);
    bool operator > (const Layer&);
    bool operator < (const Layer&);
    bool operator >= (const Layer&);
    bool operator <= (const Layer&);
    bool LowerError (int); // For checking min-layer-constraint
};

#endif // LAYER_H