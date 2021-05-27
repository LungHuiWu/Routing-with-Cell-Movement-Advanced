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
    string str_vtg_name = "";
    string str_vtg_ins = "";
    int PC = 0; int flag = 0; int blah = 0;
    while(getline(f,str)){
        if(str.substr(0,11)      == "MaxCellMove"){PC=1;}
        else if(str.substr(0,16) == "GGridBoundaryIdx"){PC=2;}
        else if(str.substr(0,8)  == "NumLayer"){PC=3;}
        else if(str.substr(0,24) == "NumNonDefaultSupplyGGrid"){PC=4;}
        else if(str.substr(0,13) == "NumMasterCell"){PC=5;}
        else if(str.substr(0,11) == "NumCellInst"){PC=6;}
        else if(str.substr(0,7)  == "NumNets"){PC=7;}
        else if(str.substr(0,15) == "NumVoltageAreas"){PC=8; readVtgArea(str);}
        else if(str.substr(0,9)  == "NumRoutes"){PC=9;}
        else if(str.substr(0,4) == "Name"){flag = 1;}
        else if(str.substr(0,9) == "Instances"){flag = 2;}
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
            if(flag == 1 && str_vtg_name == "")
            {
                str_vtg_name = str;
            }
            else if(flag == 2 && str_vtg_ins == "")
            {
                blah = 1;
                str_vtg_ins = str;
            }
            else if(flag == 1 && str_vtg_name != "")
            {
                if(blah == 1)
                {
                    readVtgArea2(str_vtg_name, str_vtg_ins);
                    str_vtg_ins = "";
                    str_vtg_name = str;
                    blah = 0;
                }
                else
                {
                    str_vtg_name = str_vtg_name + ' ' + str;
                }
            }
            else if(flag == 2 && str_vtg_ins != "")
            {
                str_vtg_ins = str_vtg_ins + ' ' + str;
            }
            break;
        case ROUTE:
            readVtgArea2(str_vtg_name, str_vtg_ins);
            readRoute(str);
            break;
        default:
            break;
        }
    }
    f.close();
    cout << "Finish Reading!!!" << endl;
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
        MCList.insert(pair<string, MCell> (name,MCell(name, p_cnt, b_cnt)));
        MCList["last"] = MCList[name];
        cout << "MasterCell " << name << " added to the design." << endl;
    }
    else if (s == "Pin")
    {
        in >> s; string name = s;
        in >> s; string layer = s;
        MCList["last"].addPin(name, *LList[layer]);
        cout << "Pin " << name << " added to MasterCell " << MCList["last"].getName() << "." << endl;
    }
    else if (s == "Blkg")
    {
        in >> s; string name = s;
        in >> s; string layer = s;
        in >> s; double d = stod(s);
        MCList["last"].addBlkg(name, *LList[layer], d);
        cout << "Blkg " << name << " added to MasterCell " << MCList["last"].getName() << "." << endl;
    }
}
void Design::readCellInst(string& str)
{
    istringstream in(str);
    string s;
    in >> s;
    if(s == "NumCellInst")
    {
        in >> s; NumCellInst = stoi(s);
        cout << "Reading CellInst..." << endl;
    }
    else if (s == "CellInst")
    {
        in >> s; string name = s;
        in >> s; string mcname = s;
        in >> s; int r = stoi(s);
        in >> s; int c = stoi(s);
        in >> s; bool mov = (s == "Movable");
        CIList.insert(pair<string, CellInst> (name, CellInst(MCList[mcname], name, r, c, mov)));
        cout << "CellInst " << name << " added to the design at (" << r << ", " << c << ")." << endl;
        // NOTE: The connection between CellInst & GGrids has not bean implement yet!!!
    }
}
void Design::readNet(string& str)
{
    istringstream in(str);
    string s;
    in >> s;
    if(s == "NumNets")
    {
        in >> s; NumNet = stoi(s);
        cout << "Reading Net..." << endl;
    }
    else if(s == "Net")
    {
        in >> s; string name = s;
        in >> s; int p = stoi(s);
        in >> s; int min = stoi(s);
        in >> s; double w = stod(s);
        NList.insert(pair<string,Net> (name, Net(name, p, min, w)));
        NList["last"] = NList[name];
        cout << "Net " << name << " added to the design." << endl;
    }
    else if (s == "Pin")
    {
        in >> s; size_t slash = s.find('/');
        string cname = s.substr(0,slash);
        string pname = s.substr(slash+1);
        vector<Pin*> p = CIList[cname].getPList();
        for (size_t i = 0; i<p.size(); ++i)
        {
            if (p[i]->getName()==pname)
            {
                p[i]->Connect();
                cout << "Pin " << pname << " in CellInst " << cname << " is connected to Net " << NList["Last"].getName() << "." <<endl;
            }
        }
    }
}
void Design::readVtgArea(string& str)
{
    istringstream in(str);
    string s;
    in >> s; in >> s; NumVtgArea = stoi(s);
    cout << "Reading VoltgeArea..." << endl;
}

void Design::readVtgArea2(string& s_name, string& s_ins)
{
    istringstream in1(s_name);
    istringstream in2(s_ins);
    string s1, s2;
    vector<string> cell;
    vector<int> x,y;
    in2 >> s2; in2 >> s2;
    while(in2 >> s2)
    {
        cell.push_back(s2);
    }
    in1 >> s1; // "Name"
    in1 >> s1; string name = s1;
    in1 >> s1; // "GGrids"
    in1 >> s1; int num = stoi(s1);
    while(in1 >> s1)
    {
        x.push_back(stoi(s1));
        in1 >> s1;
        y.push_back(stoi(s1));
    }
    for (int i = 0; i<cell.size(); ++i)
    {
        CellInst c = CIList[cell[i]];
        for (int j = 0;j<num;++j)
        {
            c.setVtgArea(x[j],y[j]);
        }
        cout << "Finish setting the VoltageArea of CellInst " << cell[i] << "." << endl;
    }
}

void Design::readRoute(string& str)
{
    istringstream in(str);
    string s;
    in >> s;
    if(s == "NumRoutes")
    {
        in >> s; NumRoute = stoi(s);
        cout << "Reading Route..." << endl;
    }
    else
    {
        in >> s; int r1 = stoi(s);
        in >> s; int c1 = stoi(s);
        in >> s; int l1 = stoi(s);
        in >> s; int r2 = stoi(s);
        in >> s; int c2 = stoi(s);
        in >> s; int l2 = stoi(s);
        in >> s; string n = s;
        NList[n].addRoute(r1,c1,l1,r2,c2,l2);
        cout << "Route added to Net " << n << "." << endl;
    }
}

