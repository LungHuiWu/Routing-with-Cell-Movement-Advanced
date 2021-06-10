#ifndef ROUTE_H
#define ROUTE_H
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Route
{
private:
    int RowS, ColS, LyrS;
    int RowE, ColE, LyrE;
    string NetName;
    vector<tuple<int, int>> p; //the vector include start point and end point
public:
    Route();
    Route(int, int, int, int, int, int, string);
    ~Route();
    vector<tuple<int, int>> getPoints();
};

#endif // ROUTE_H
