#include <string>
#include <iostream>
#include "Net.h"

using namespace std;

Net::Net()
{
    NumPin = 0;
    MinLayer = 0;
    Weight = 0;
}

Net::Net(string name, int num, int min, int w)
{
    Name = name;
    NumPin = num;
    MinLayer = min;
    Weight = w;
}

Net::~Net()
{
}

void Net::connect(CellInst c, string pinname)
{
    vector<Pin> p = c.getPList();
    for(int i=0; i<p.size(); i++){
        if(p[i].getName()==pinname){
            p[i].Connect();
        }
    }
}

void Net::addRoute(int rs, int cs, int ls ,int re, int ce, int le)
{
    RList.push_back(Route(rs,cs,ls,re,ce,le,Name));
}