#include <string>
#include <iostream>
#include "Net.h"
#include "CellInst.h"

using namespace std;

Net::Net()
{
    NumPin = 0;
    MinLayer = 0;
    Weight = 0;
}

Net::Net(string name, int num, int min, double w)
{
    Name = name;
    NumPin = num;
    MinLayer = min;
    Weight = w;
}

Net::~Net()
{
}

void Net::connect(CellInst c, string pinname) //may be considered with the connect in pin
{
    vector<Pin*> p = c.getPList();
    for(int i=0; i<p.size(); i++){
        if(p[i]->getName()==pinname){
            p[i]->Connect(Name);
            auto CItoNet = find(conCIs.begin(), conCIs.end(), c.getCIName());
            if (CItoNet == conCIs.end())
            {
                conCIs.push_back(c.getCIName());
            }
        }
    }
}

void Net::Disconnect(CellInst c, string pinname) //may be considered with the connect in pin
{
    vector<Pin*> p = c.getPList();
    for(int i=0; i<p.size(); i++){
        if(p[i]->getName()==pinname){
            p[i]->Disconnect();
            auto CItoNet = find(conCIs.begin(), conCIs.end(), c.getCIName());
            if (CItoNet != conCIs.end())
            {
                auto i = find(conCIs.begin(), conCIs.end(), c.getCIName());
                conCIs.erase(i);
            }
        }
    }
}
void Net::addRoute(int rs, int cs, int ls ,int re, int ce, int le)
{
    RList.push_back(new Route(rs,cs,ls,re,ce,le,Name));
}

Net& Net::operator = (const Net& n)
{
    Name = n.Name;
    NumPin = n.NumPin;
    MinLayer = n.MinLayer;
    Weight = n.Weight;
    RList = n.RList;
    return *this;
}

string Net::getName(){
    return Name;
}

double Net::getWeight(){
    return Weight;
}

vector<string> Net::getCIs()
{
    return conCIs;
}

vector<Route*> Net::delRoute(CellInst c, tuple<int, int> p, map<string,CellInst> CIList)
{
    for(int i= 0; i<RList.size(); )
    {
        if(get<0>(RList[i]->getPoints()[0])==get<0>(p) && get<1>(RList[i]->getPoints()[0])==get<1>(p))
        {
            int ConnecttoCI = 0;
            R.push_back(RList[i]);
            RList.erase(RList.begin()+i);
            for(int i=0; i<conCIs.size(); i++)
            {
                if(CIList[conCIs[i]].getLocation() == make_tuple(get<0>(RList[i]->getPoints()[1]), get<1>(RList[i]->getPoints()[1])))
                {
                    return R;
                }
            }
            delRoute(c, make_tuple(get<0>(RList[i]->getPoints()[1]), get<1>(RList[i]->getPoints()[1])), CIList);    
        }
        else if (get<0>(RList[i]->getPoints()[1])==get<0>(p) && get<1>(RList[i]->getPoints()[1])==get<1>(p))
        {
            int ConnecttoCI = 0;
            R.push_back(RList[i]);
            RList.erase(RList.begin()+i);
            for(int i=0; i<conCIs.size(); i++)
            {
                if(CIList[conCIs[i]].getLocation() == make_tuple(get<0>(RList[i]->getPoints()[0]), get<1>(RList[i]->getPoints()[0])))
                {
                    return R;
                }
            }
            delRoute(c, make_tuple(get<0>(RList[i]->getPoints()[0]), get<1>(RList[i]->getPoints()[0])), CIList); 
        }   
        else
        {
            i ++ ;
        }
    }
}

vector<Route*> Net::getRList()
{
    return RList;
}

