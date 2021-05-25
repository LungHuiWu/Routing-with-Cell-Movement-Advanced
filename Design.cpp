#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Design.h"

using namespace std;

Design::Design()
{
}

Design::~Design()
{
}

enum NowPC{
    MAXCELLMOVE = 1,
    GGRIDBOUND,
    LAYER,
    NONDEFAULT,
    MCELL,
    CELLINST,
    NET,
    VTGAREA,
    ROUTE,
};

bool Design::readFile(const string& file)
{
    fstream f;
    f.open(file, ios::in);
    if (!f)
    {
        cerr << "Can't read file!!!" << endl;
        return false;
    }
    string str;
    int PC = 0;
    while(getline(f,str)){
        if(str.substr(0,11)      == "MaxCellMove"){PC=1;}
        else if(str.substr(0,16) == "GGridBoundaryIdx"){PC=2;}
        else if(str.substr(0,8)  == "NumLayer"){PC=3;}
        else if(str.substr(0,24) == "NumNonDefaultSupplyGGrid"){PC=4;}
        else if(str.substr(0,13) == "NumMasterCell"){PC=5;}
        else if(str.substr(0,26) == "NumNeighborCellExtraDemand"){PC=6;}
        else if(str.substr(0,11) == "NumCellInst"){PC=7;}
        else if(str.substr(0,7)  == "NumNets"){PC=8;}
        else if(str.substr(0,9)  == "NumRoutes"){PC=9;}
        switch (PC)
        {
        case MAXCELLMOVE:
            readMaxCellMove(str);
            break;
        case GGRIDBOUND:
            readGGridBoundaryIdx(str);
            break;
        case LAYER:
            readLayer(str);
            break;
        case NONDEFAULT:
            readNonDefaultGGrid(str);
            break;
        case MCELL:
            readMCell(str);
            break;
        case CELLINST:
            readCellInst(str);
            break;
        case NET:
            readNet(str);
            break;
        case VTGAREA:
            readVtgArea(str);
            break;
        case ROUTE:
            readRoute(str);
            break;
        default:
            break;
        }
    }
    f.close();
    cout << "Finish Reading" << endl;
    return true;
}
void Design::readMaxCellMove(string& str)
{
    cout << "Reading MaxCellMove..." << endl;
    MaxCellMove = stoi(str.substr(12));
}

void Design::readGGridBoundaryIdx(string& str)
{
    cout << "Reading GGridBoundaryIdx..." << endl;
    string s;
    istringstream in(str);
    in >> s; // GGridBoundaryIdx
    in >> s; RowMin = stoi(s); // rowBeginIdx
    in >> s; ColMin = stoi(s); // colBeginIdx
    in >> s; RowMax = stoi(s); // rowEndIdx
    in >> s; ColMax = stoi(s); // colEndIdx
    NumRow = RowMax - RowMin + 1;
    NumCol = ColMax - ColMin + 1;
}
void Design::readLayer(string& str)
{
    istringstream in(str);
    string s;
    in >> s;
    if (s == "NumLayer")
    {
        cout << "Reading Layer..." << endl;
        in >> s;
        NumLyr = stoi(s);
    }
    else
    {
        in >> s; string name = s;
        in >> s; int idx = stoi(s);
        in >> s; char dir = s[0];
        in >> s; int supply = stoi(s);
        in >> s; double power = stod(s);
        LList.insert(pair<string, Layer*> (name, new Layer(name, idx, dir, supply, power)));
        cout << "Layer " << idx << "("<< name << ") added to the design!" << endl;
    }
}
void Design::readNonDefaultGGrid(string& str)
{
    istringstream in(str);
    string s;
    in >> s;
    if (s == "NumNonDefaultSupplyGGrid")
    {
        cout << "Reading GGrid..." << endl;
        in >> s; NumNonDefaultSupplyGGrid = stoi(s);
        // build GGrids
        GGridList = new GGrid**[NumRow];
        for (size_t i = 0; i < NumRow; ++i)
        {
            GGridList[i] = new GGrid*[NumCol];
            for (size_t j = 0; j < NumCol; ++j)
            {
                GGridList[i][j] = new GGrid[NumLyr];
                for (const auto& l : LList)
                {
                    GGridList[i][j][l.second->getIdx()-1] = GGrid(i+1, j+1, l.second);
                }
            }
        }
        cout << "All GGrids constructed!" << endl;
    }
    else
    {
        int x = stoi(s);
        in >> s; int y = stoi(s);
        in >> s; int z = stoi(s);
        in >> s;
        GGridList[x][y][z].adjustSupply(stoi(s));
        cout << "Done adjusting non-default supplies!" << endl;
    }
}
void Design::readMCell(string& str)
{
    istringstream in(str);
    string s;
    in >> s;
    if (s == "NumMasterCell")
    {
        cout << "Reading MCell..." << endl;
        in >> s; NumMCell = stoi(s);
    }
    else if (s == "MasterCell")
    {
        in >> s; string name = s;
        in >> s; int p_cnt = stoi(s);
        in >> s; int b_cnt = stoi(s);
        MCList.push_back(MCell(name, p_cnt, b_cnt));
        cout << "MasterCell " << name << " added to the design." << endl;
    }
    else if (s == "Pin")
    {
        in >> s; string name = s;
        in >> s; string layer = s;
        MCList.back().addPin(name, *LList[layer]);
        cout << "Pin " << name << " added to MasterCell " << MCList.back().getName() << "." << endl;
    }
    else if (s == "Blkg")
    {
        in >> s; string name = s;
        in >> s; string layer = s;
        in >> s; double d = stod(s);
        MCList.back().addBlkg(name, *LList[layer], d);
        cout << "Blkg " << name << " added to MasterCell " << MCList.back().getName() << "." << endl;
    }
}
void Design::readCellInst(string& str){}
void Design::readNet(string& str){}
void Design::readVtgArea(string& str){}
void Design::readRoute(string& str){}

// following functions are unnecessary
void Design::setMaxCellMove(int max)
{
    MaxCellMove = max;
}
void Design::setBoundary(int a, int b, int c, int d)
{
    RowMin = a;
    ColMin = b;
    RowMax = c;
    ColMax = d;
}
void Design::setNumLyr(int l)
{
    NumLyr = l;
}
void Design::setNumNonDefaultSupplyGGrid(int n)
{
    NumNonDefaultSupplyGGrid = n;
}
void Design::setNumMCell(int mc)
{
    NumMCell = mc;
}
void Design::setNumCellInst(int cell)
{
    NumCellInst = cell;
}
void Design::setNumNet(int  n)
{
    NumNet = n;
}
void Design::setNumVtgArea(int va)
{
    NumVtgArea = va;
}
void Design::setNumRoute(int r)
{
    NumRoute = r;
}
