#ifndef ROUTE_H
#define ROUTE_H
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Route
{
private:
    vector<tuple<int, int, int>> p; //the vector include start point and end point
public:
    int RowS, ColS, LyrS;
    int RowE, ColE, LyrE;
    string NetName; 
    string LayerS;
    string LayerE;
    Route();
    Route(int, int, int, int, int, int, string);
    ~Route();
    vector<tuple<int, int, int>> getPoints();
};

#endif // ROUTE_H
