#include <string>
#include <iostream>
#include "Route.h"

using namespace std;

Route::Route()
{
    RowS = 0;
    ColS = 0;
    LyrS = 0;
    RowE = 0;
    ColE = 0;
    LyrE = 0;
}

Route::Route(int rs, int cs, int ls, int re, int ce, int le, string netname)
{
    RowS = rs;
    ColS = cs;
    LyrS = ls;
    RowE = re;
    ColE = ce;
    LyrE = le;
<<<<<<< HEAD
    p.push_back(make_tuple(RowS,ColS));
    p.push_back(make_tuple(RowE,ColE));
=======
    NetName = netname;
>>>>>>> e271ea4cbe59c9cb85c2825563bfccd0a06864af
}

Route::~Route()
{
}

vector<tuple<int, int>> Route::getPoints()
{
    return p;
}