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
    int Powerfactor;

public:
    Layer();
    Layer(string, int, char, int, int);
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

