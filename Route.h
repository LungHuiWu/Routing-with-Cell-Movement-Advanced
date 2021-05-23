#include <string>

using namespace std;

class Route
{
private:
    int RowS, ColS, LyrS;
    int RowE, ColE, LyrE;
    string NetName;
public:
    Route();
    Route(int, int, int, int, int, int, string);
    ~Route();
};

