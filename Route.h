#ifndef ROUTE_H
#define ROUTE_H
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Route
{
<<<<<<< HEAD
private:
    vector<tuple<int, int>> p; //the vector include start point and end point
=======
>>>>>>> e271ea4cbe59c9cb85c2825563bfccd0a06864af
public:
    int RowS, ColS, LyrS;
    int RowE, ColE, LyrE;
    string NetName;
<<<<<<< HEAD
=======
    string LayerS;
    string LayerE;
>>>>>>> e271ea4cbe59c9cb85c2825563bfccd0a06864af
    Route();
    Route(int, int, int, int, int, int, string);
    ~Route();
    vector<tuple<int, int>> getPoints();
};

#endif // ROUTE_H
