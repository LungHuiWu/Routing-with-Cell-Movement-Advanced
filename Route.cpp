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
}

Route::~Route()
{
}