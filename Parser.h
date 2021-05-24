#include <string>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>

using namespace std;

class Parser
{
private:
    int MaxCellMove;
    int rowBeginIdx, colBeginIdx, rowEndIdx, colEndIdx;
    int NumLayer;
    vector<tuple<string, int, char, int, int>> LList;
    int NumNonDefaultSupplyGGrid;
    vector<tuple<int, int, int, int>> NonDefaultSupplyGGridList;
    int NumMCell;

public:
    Parser(/* args */);
    ~Parser();
};

Parser::Parser(/* args */)
{
}

Parser::~Parser()
{
}
