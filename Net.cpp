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

void Net::delRoute(CellInst c, tuple<int, int> p, map<string,CellInst> CIList, vector<tuple<string,string,string>>& RT)
{
    for(int i= 0; i<RList.size(); ++i)
    {
        bool cont = true;
        if(RList[i]->getPoints()[0]==p)
        {
            R.push_back(RList[i]);
            if (RList[i]->getPoints()[0]!=RList[i]->getPoints()[1])
            {
                for(int k=0; k<conCIs.size(); k++)
                {
                    if(CIList[conCIs[k]].getLocation() == RList[i]->getPoints()[1])
                    {
                        cont = false;
                        for (int j=0;j<CIList[conCIs[k]].getPList().size();++j)
                        {
                            if(CIList[conCIs[k]].getPList()[j]->getNetname() == Name)
                            {
                                RT.push_back(make_tuple(Name, conCIs[k], CIList[conCIs[k]].getPList()[j]->getName()));
                                CIList[conCIs[k]].getPList()[j]->Disconnect();
                            }
                        }
                    }
                }
            }
            else cont = false;
            tuple<int,int> temp = RList[i]->getPoints()[1];
            RList.erase(RList.begin()+i);
            if (cont)
            {
                delRoute(c, temp, CIList, RT);    
            }
        }
        else if (RList[i]->getPoints()[1]==p)
        {
            R.push_back(RList[i]);
            if (RList[i]->getPoints()[0]!=RList[i]->getPoints()[1])
            {
                for(int k=0; k<conCIs.size(); k++)
                {
                    if(CIList[conCIs[k]].getLocation() == RList[i]->getPoints()[0])
                    {
                        cont = false;
                        // disconnect CI at the other end
                        // Risk: maybe there is a net using more than one pin in one CI, this alg. will disconnect all pins in the net  on CI
                        for (int j=0;j<CIList[conCIs[k]].getPList().size();++j)
                        {
                            if(CIList[conCIs[k]].getPList()[j]->getNetname() == Name)
                            {
                                RT.push_back(make_tuple(Name, conCIs[k], CIList[conCIs[k]].getPList()[j]->getName()));
                                CIList[conCIs[k]].getPList()[j]->Disconnect();
                            }
                        }
                    }
                }
            }
            else cont = false;
            tuple<int,int> temp = RList[i]->getPoints()[0];
            RList.erase(RList.begin()+i);
            if (cont)
            {
                delRoute(c, temp, CIList, RT);
            }
        }
    }
    return;
}

vector<Route*> Net::getRList()
{
    return RList;
}

