#ifndef ROUTE_H
#define ROUTE_H
#include <string>

using namespace std;

class Route
{
public:
    int RowS, ColS, LyrS;
    int RowE, ColE, LyrE;
    string NetName;
    string LayerS;
    string LayerE;
    Route();
    Route(int, int, int, int, int, int, string);
    ~Route();
};

#endif // ROUTE_H
