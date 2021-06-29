#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
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
    string MCN = ""; string NN = "";
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
            MCN = readMCell(str, MCN);
            break;
        case CELLINST:
            readCellInst(str);
            break;
        case NET:
            NN = readNet(str, NN);
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
            readRoute(str);
            break;
        default:
            break;
        }
    }
    readVtgArea2(str_vtg_name, str_vtg_ins);
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
        Layer* l = new Layer(name, idx, dir, supply, power);
        LList.insert(pair<string, Layer*> (name, l));
        LList_idx[idx] = l;
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
        cout << "Done adjusting non-default supplies at (" << x <<  "," << y << "," << z << ")!" << endl;
    }
}

string Design::readMCell(string& str, string& MCname)
{
    istringstream in(str);
    string s;
    in >> s;
    if (s == "NumMasterCell")
    {
        cout << "Reading MCell..." << endl;
        in >> s; NumMCell = stoi(s);
        return MCname;
    }
    else if (s == "MasterCell")
    {
        in >> s; string name = s;
        in >> s; int p_cnt = stoi(s);
        in >> s; int b_cnt = stoi(s);
        MCList.insert(pair<string, MCell> (name,MCell(name, p_cnt, b_cnt)));
        cout << "MasterCell " << name << " added to the design." << endl;
        return name;
    }
    else if (s == "Pin")
    {
        in >> s; string name = s;
        in >> s; string layer = s;
        MCList[MCname].addPin(name, *LList[layer]);
        cout << "Pin " << name << " added to MasterCell " << MCList[MCname].getName() << "." << endl;
        return MCname;
    }
    else if (s == "Blkg")
    {
        in >> s; string name = s;
        in >> s; string layer = s;
        in >> s; double d = stod(s);
        MCList[MCname].addBlkg(name, *LList[layer], d);
        cout << "Blkg " << name << " added to MasterCell " << MCList[MCname].getName() << "." << endl;
        return MCname;
    }
    return MCname;
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
        if(mov == true)
        {
            mCIList.insert(pair<string, CellInst> (name, CellInst(MCList[mcname], name, r, c, mov)));
        }
        CIList.insert(pair<string, CellInst> (name, CellInst(MCList[mcname], name, r, c, mov)));
        for (int i=0;i<MCList[mcname].getBList().size();++i)
        {
            GGridList[r-1][c-1][MCList[mcname].getBList()[i]->getLayer().getIdx()-1].adjustSupply(-MCList[mcname].getBList()[i]->getDemand());
        }
        cout << "CellInst " << name << " added to the design at (" << r << ", " << c << ")." << endl;
        // NOTE: The connection between CellInst & GGrids has not bean implement yet!!!
    }
}

string Design::readNet(string& str, string& Nname)
{
    istringstream in(str);
    string s;
    in >> s;
    if(s == "NumNets")
    {
        in >> s; NumNet = stoi(s);
        cout << "Reading Net..." << endl;
        return Nname;
    }
    else if(s == "Net")
    {
        in >> s; string name = s;
        for (int i=0;i<NumRow;++i){
            for (int j=0;j<NumCol;++j){
                for (int k=0;k<NumLyr;++k){
                    GGridList[i][j][k].addNet(name);
                }
            }
        }
        in >> s; int p = stoi(s);
        in >> s; int min;
        if (s != "NoCstr") min = LList[s]->getIdx();
        else min = 0;
        in >> s; double w = stod(s);
        NList.insert(pair<string,Net> (name, Net(name, p, min, w)));
        cout << "Net " << name << " added to the design." << endl;
        return name;
    }
    else if (s == "Pin")
    {
        in >> s; size_t slash = s.find('/');
        string cname = s.substr(0,slash);
        string pname = s.substr(slash+1);
        for (size_t i = 0; i<CIList[cname].getPList().size(); ++i)
        {
            if (CIList[cname].getPList()[i]->getName()==pname)
            {
                //CIList[cname].getPList()[i]->Connect(Nname);
                NList[Nname].connect(CIList[cname], pname);
                cout << "Pin " << pname << " in CellInst " << cname << " is connected to Net " << Nname << "." <<endl;
            }
        }
        return Nname;
    }
    return Nname;
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
        int r1 = stoi(s);
        in >> s; int c1 = stoi(s);
        in >> s; int l1 = stoi(s);
        in >> s; int r2 = stoi(s);
        in >> s; int c2 = stoi(s);
        in >> s; int l2 = stoi(s);
        in >> s; string n = s;
        NList[n].addRoute(r1,c1,l1,r2,c2,l2);
        addRoute(r1,r2,c1,c2,l1,l2,n); // GGrid update supply
        cout << "Route added to Net " << n << "." << endl;
    }
}

string Design::select()
{   
    cout << "Start selecting..." << endl;
    // reset all nets' R
    for(auto& n : NList)
    {
        n.second.clearR();
    }
    double maxWeight = 0;
    string CI;
    for(auto& c : mCIList)
    {
        double Weight = 0;
        for(int i = 0; i < c.second.getPList().size(); i++)
        {   
            string net = c.second.getPList()[i]->getNetname();
            if(net != "")
            {
                Weight += NList[net].getWeight();
            }
        }
        if(Weight >= maxWeight)
        {
            if(find(selected.begin(), selected.end(), c.second.getCIName())==selected.end()){
                maxWeight = Weight;
                CI = c.second.getCIName();
            }
            
            //cout <<"current CI is "<<CI<<" with weight "<<Weight<<"."<<endl;
        }
    }
    selected.push_back(CI);
    ADJCIs = CIList[CI].getADJCIs(NList); // All CIs connected to CI
    adjNets = CIList[CI].getADJNets(); // All nets connected to CI

    for(int i = 0; i<CIList[CI].getPList().size(); i++)
    {
        tuple<int, int, int> p = make_tuple(get<0>(CIList[CI].getLocation()), get<1>(CIList[CI].getLocation()), CIList[CI].getPList()[i]->getLayer().getIdx());
        string net = CIList[CI].getPList()[i]->getNetname();
        NList[net].Disconnect(CIList[CI],CIList[CI].getPList()[i]->getName());
        NList[net].delRoute(CIList[CI], p, CIList); //Routes which were deleted
        for(auto& r : NList[net].getR())
        {
            delRoute(r,net);
        }
    }
    return CI;
}

void Design::addRoute(int r1, int r2, int c1, int c2, int l1, int l2, string n)
{
    if (r1 < r2)
    {
        for (int i=r1;i<=r2;++i)
        {
            GGridList[i-1][c1-1][l1-1].linkNet(NList[n].getName());
        }
    }
    else if (r1 > r2)
    {
        for (int i=r2;i<=r1;++i)
        {
            GGridList[i-1][c1-1][l1-1].linkNet(NList[n].getName());
        }
    }
    else if (c1 < c2)
    {
        for (int i=c1;i<=c2;++i)
        {
            GGridList[r1-1][i-1][l1-1].linkNet(NList[n].getName());
        }
    }
    else if (c1 > c2)
    {
        for (int i=c2;i<=c1;++i)
        {
            GGridList[r1-1][i-1][l1-1].linkNet(NList[n].getName());
        }
    }
    else if (l1 < l2)
    {
        for (int i=l1;i<=l2;++i)
        {
            GGridList[r1-1][c1-1][i-1].linkNet(NList[n].getName());
        }
    }
    else if (l1 > l2)
    {
        for (int i=l2;i<=l1;++i)
        {
            GGridList[r1-1][c1-1][i-1].linkNet(NList[n].getName());
        }
    }
    else
    {
        cout << "???" << endl;
    }
}

void Design::delRoute(Route* r, string n)
{
    int r1 = r->RowS; int r2 = r->RowE;
    int c1 = r->ColS; int c2 = r->ColE;
    int l1 = r->LyrS; int l2 = r->LyrE;
    if (r1 < r2)
        {
            for (int i=r1;i<=r2;++i)
            {
                GGridList[i-1][c1-1][l1-1].unlinkNet(NList[n].getName());
            }
        }
        else if (r1 > r2)
        {
            for (int i=r2;i<=r1;++i)
            {
                GGridList[i-1][c1-1][l1-1].unlinkNet(NList[n].getName());
            }
        }
        else if (c1 < c2)
        {
            for (int i=c1;i<=c2;++i)
            {
                GGridList[r1-1][i-1][l1-1].unlinkNet(NList[n].getName());
            }
        }
        else if (c1 > c2)
        {
            for (int i=c2;i<=c1;++i)
            {
                GGridList[r1-1][i-1][l1-1].unlinkNet(NList[n].getName());
            }
        }
        else if (l1 < l2)
        {
            for (int i=l1;i<=l2;++i)
            {
                GGridList[r1-1][c1-1][i-1].unlinkNet(NList[n].getName());
            }
        }
        else if (l1 > l2)
        {
            for (int i=l2;i<=l1;++i)
            {
                GGridList[r1-1][c1-1][i-1].unlinkNet(NList[n].getName());
            }
        }
        else
        {
            cout << "???" << endl;
        }
}

void Design::clearGGridstep()
{
    for (int i=0;i<NumRow;++i)
    {
        for (int j=0;j<NumCol;++j)
        {
            for (int k=0;k<NumLyr;++k)
            {
                GGridList[i][j][k].Step = 0;
            }
        }
    }
}

void Design::clearGGridCovered()
{
    for (int i=0;i<NumRow;++i)
    {
        for (int j=0;j<NumCol;++j)
        {
            for (int k=0;k<NumLyr;++k)
            {
                GGridList[i][j][k].Covered = 0;
            }
        }
    }
}

void Design::clearAncestor()
{
    for (int i=0;i<NumRow;++i)
    {
        for (int j=0;j<NumCol;++j)
        {
            for (int k=0;k<NumLyr;++k)
            {
                GGridList[i][j][k].Ancestor = make_tuple(-1,-1,-1);
            }
        }
    }
}

void Design::setGGridCovered(Route* r, int idx)
{
    int r1 = r->RowS; int r2 = r->RowE;
    int c1 = r->ColS; int c2 = r->ColE;
    int l1 = r->LyrS; int l2 = r->LyrE;
    if (r1 < r2)
    {
        for (int i=r1;i<=r2;++i)
        {
            GGridList[i-1][c1-1][l1-1].Covered = idx;
        }
    }
    else if (r1 > r2)
    {
        for (int i=r2;i<=r1;++i)
        {
            GGridList[i-1][c1-1][l1-1].Covered = idx;
        }
    }
    else if (c1 < c2)
    {
        for (int i=c1;i<=c2;++i)
        {
            GGridList[r1-1][i-1][l1-1].Covered = idx;
        }
    }
    else if (c1 > c2)
    {
        for (int i=c2;i<=c1;++i)
        {
            GGridList[r1-1][i-1][l1-1].Covered = idx;
        }
    }
    else if (l1 < l2)
    {
        for (int i=l1;i<=l2;++i)
        {
            GGridList[r1-1][c1-1][i-1].Covered = idx;
        }
    }
    else if (l1 > l2)
    {
        for (int i=l2;i<=l1;++i)
        {
            GGridList[r1-1][c1-1][i-1].Covered = idx;
        }
    }
    else
    {
        cout << "???" << endl;
    }
}

void Design::showCovered()
{
    for (int k=0;k<NumLyr;++k)
    {
        for(int i=0;i<NumRow;++i)
        {
            for (int j=0;j<NumCol;++j)
            {
                cout << GGridList[i][j][k].Covered;
            }
            cout<< endl;
        }
        cout << endl;
    }
}

void Design::showColor()
{
    for (int k=0;k<NumLyr;++k)
    {
        for(int i=0;i<NumRow;++i)
        {
            for (int j=0;j<NumCol;++j)
            {
                cout << GGridList[i][j][k].Color;
            }
            cout<< endl;
        }
        cout << endl;
    }
}

void Design::showStep()
{
    for (int k=0;k<NumLyr;++k)
    {
        for(int i=0;i<NumRow;++i)
        {
            for (int j=0;j<NumCol;++j)
            {
                cout << GGridList[i][j][k].Step;
            }
            cout<< endl;
        }
        cout << endl;
    }
}

void Design::setWhite()
{
    for (int i=0;i<NumRow;++i)
    {
        for (int j=0;j<NumCol;++j)
        {
            for (int k=0;k<NumLyr;++k)
            {
                GGridList[i][j][k].Color = "W";
            }
        }
    }
}

void Design::setCICovered(vector<Route*>& RList, int row,int c,int l,int idx)
{
    if (RList.empty())
    {
        GGridList[row-1][c-1][l-1].Covered = idx;
        return;
    }
    for (int i=0;i<RList.size();++i)
    {
        Route* r = RList[i];
        if (r->getPoints()[0] == make_tuple(row,c,l))
        {
            setGGridCovered(r,idx);
            RList.erase(RList.begin()+i);
            setCICovered(RList,get<0>(r->getPoints()[1]),get<1>(r->getPoints()[1]),get<2>(r->getPoints()[1]),idx);
            return;
        }
        else if (r->getPoints()[1] == make_tuple(row,c,l))
        {
            setGGridCovered(r,idx);
            RList.erase(RList.begin()+i);
            setCICovered(RList,get<0>(r->getPoints()[0]),get<1>(r->getPoints()[0]),get<2>(r->getPoints()[0]),idx);
            return;
        }
    }
    GGridList[row-1][c-1][l-1].Covered = idx;
    return;
}

vector<tuple<int,int>> Design::placement(string& CI)
{   
    //case1 : CI doesn't have votage area, do BFS
    if(CIList[CI].hasVtgArea() == false)
    {
        cout<<CI<<" doesn't has votage area."<<endl;
        int numinp = 0;
        vector<tuple<int,int>> p;
        //vector<vector<tuple<int,int>>> subnet;
        //cout<<subnet.size()<<endl;
        int nsize=adjNets.size();
        vector<vector<tuple<int,int>>> subnet(nsize);
        for(int N=0; N<subnet.size(); N++)
        {
            vector<Route*> r = NList[adjNets[N]].getRList();
            for(int R=0; R<r.size(); R++)
            {
                int x1 = get<0>(r[R]->getPoints()[0]);
                int y1 = get<1>(r[R]->getPoints()[0]);
                int x2 = get<0>(r[R]->getPoints()[1]);
                int y2 = get<1>(r[R]->getPoints()[1]);
                if(x1==x2 && y1==y2)
                {
                    if (find(subnet[N].begin(), subnet[N].end(), make_tuple(x1, y1))==subnet[N].end()) 
                    {
                        subnet[N].push_back(make_tuple(x1,y1));
                    }
                }
                else if(x1==x2)
                {
                    if(y1<=y2)
                    {
                        for(int i=y1; i<=y2; i++) 
                        {
                            if (find(subnet[N].begin(), subnet[N].end(), make_tuple(x1, i))==subnet[N].end())
                            {
                                subnet[N].push_back(make_tuple(x1,i));
                            }
                        }
                    }
                    else
                    {
                        for(int i=y2; i<=y1; i++) 
                        {
                            if (find(subnet[N].begin(), subnet[N].end(), make_tuple(x1, i))==subnet[N].end())
                            {
                                subnet[N].push_back(make_tuple(x1,i));
                            }
                        }
                    }
                }
                else if(y1==y2)
                {
                    if(x1<=x2)
                    {
                        for(int i=x1; i<=x2; i++) 
                        {
                            if (find(subnet[N].begin(), subnet[N].end(), make_tuple(i, y1))==subnet[N].end())
                            {
                                subnet[N].push_back(make_tuple(i,y1));
                            }
                        }
                    }
                    else
                    {
                        for(int i=x2; i<=x1; i++) 
                        {
                            if (find(subnet[N].begin(), subnet[N].end(), make_tuple(i, y1))==subnet[N].end())
                            {
                                subnet[N].push_back(make_tuple(i,y1));
                            }
                        }
                    }
                }
            }
        }
        bool nosubnet = true;
        for(int i=0; i<subnet.size(); i++)
        {
            if(subnet[i].size()!=0) nosubnet = false;
        }
        if(nosubnet == true)
        {
            cout <<CI<<" has no subnet."<<endl;
            p.push_back(CIList[CI].getLocation());
            p.push_back(CIList[CI].getLocation());
            return p;
        }
        vector<vector<tuple<int,int>>> newvisited = subnet;
        vector<vector<tuple<int,int>>> visited = subnet;
        while(numinp < 2)
        {
            for(int N=0; N<subnet.size(); N++)
            {
                int l=0;
                int loop=newvisited[N].size();
                while(l<loop)
                {
                    int x=get<0>(newvisited[N][l]);
                    int y=get<1>(newvisited[N][l]);
                    if(x<RowMax)
                    {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x+1, y))==subnet[N].end())
                        {  
                            newvisited[N].push_back(make_tuple(x+1, y));
                            subnet[N].push_back(make_tuple(x+1, y));
                            visited[N].push_back(make_tuple(x+1, y));
                        }
                    }
                    if(x>RowMin)
                    {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x-1, y))==subnet[N].end())
                        {
                            newvisited[N].push_back(make_tuple(x-1, y));
                            subnet[N].push_back(make_tuple(x-1, y));
                            visited[N].push_back(make_tuple(x-1, y));
                        }
                    }
                    if(y<ColMax)
                    {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x, y+1))==subnet[N].end())
                        {
                            newvisited[N].push_back(make_tuple(x, y+1));
                            subnet[N].push_back(make_tuple(x, y+1));
                            visited[N].push_back(make_tuple(x, y+1));
                        }
                    }
                    if(y>ColMin)
                        {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x, y-1))==subnet[N].end())
                        {
                            newvisited[N].push_back(make_tuple(x, y-1));
                            subnet[N].push_back(make_tuple(x, y-1));
                            visited[N].push_back(make_tuple(x, y-1));
                        }
                    }
                    l++;
                }
                newvisited[N].erase(newvisited[N].begin(), newvisited[N].begin()+loop);
            }
             
            int nenet = 0;
            for(int i=0; i<visited.size(); i++)
            {
                if(visited[i].size()!=0)
                {
                    nenet = i;
                    break;
                }
            }
            
            for(int i=0; i<visited[nenet].size(); i++)
            {
                if(numinp==2)
                {
                    break;
                }
                bool overlap=true;
                bool pointvalid = true;
                int row = get<0>(visited[nenet][i]);
                int col = get<1>(visited[nenet][i]);
                for(int k=0; k<CIList[CI].getBList().size(); k++)
                {
                    int lyr=CIList[CI].getBList()[k]->getLayer().getIdx();
                    if(GGridList[row-1][col-1][lyr-1].getSupply()<=CIList[CI].getBList()[k]->getDemand())
                    {
                        pointvalid = false;
                        cout << "invalid point"<<endl;
                    }
                }
                
                if(pointvalid==false) continue; //this point is not valid 
                for(int j=1; j<visited.size(); j++)
                {
                    if(visited[j].size()!=0)
                    {
                        if(find(visited[j].begin(), visited[j].end(), visited[nenet][i])==visited[j].end()) 
                        {
                            overlap=false;
                                  
                        }
                        
                    }
                }
                if(overlap==true && find(p.begin(), p.end(), visited[nenet][i])==p.end())
                {
                    p.push_back(visited[nenet][i]);
                    numinp += 1;
                }
            }
        }
        return p;
    }
    //case2 : CI has votage area, find min dis
    else
    {
        vector<tuple<int,int>> p(2); //new places
        vector<tuple<int,int>> Vtgarea = CIList[CI].getVtgArea();
        int mindis = numeric_limits<int>::max();
        int secmindis = numeric_limits<int>::max();
        vector<string> c = CIList[CI].getADJCIs(NList); //find CIs connected to CI
        for(int i = 0; i < Vtgarea.size(); i++)
        {
            int dis = 0;
            int row = get<0>(Vtgarea[i]);
            int col = get<1>(Vtgarea[i]);
            bool pointvalid = true;
            for(int i=0; i<CIList[CI].getBList().size(); i++)
            {
                int lyr=CIList[CI].getBList()[i]->getLayer().getIdx();
                if(GGridList[row-1][col-1][lyr-1].getSupply()<=CIList[CI].getBList()[i]->getDemand())
                {
                    pointvalid = false;
                }
            }
            if(pointvalid==false) continue; //this point is not valid 
            for(int i = 0; i<c.size(); i++)
            {
                dis += abs(row-get<0>(CIList[c[i]].getLocation()));
                dis += abs(col-get<1>(CIList[c[i]].getLocation()));
            }
            if (dis<mindis)
            {
                mindis = dis;
                p[0] = Vtgarea[i];    
                
            }
            else if(dis>=mindis && dis<=secmindis)
            {

                secmindis = dis;
                p[1] = Vtgarea[i];

            }
                
        }
        return p;
    }
}

vector<Route*> Design::mst(int count, int r,int c,int l, int minlyr, string netname)
{
    vector<Route*> outputRoute;
    vector<tuple<int,int,int>> queue1, queue2, queue3, queue4;
    int row, col, lyr;
    bool found;
    while(count >1)
    {
        queue1.clear(); // now Q
        queue2.clear(); // outer part
        queue3.clear(); // points ready to be linked
        queue4.clear(); // every point waiting to be added as route
        found = false;
        setWhite(); // color all grid white
        clearGGridstep(); // set all step to 0
        clearAncestor(); // set all ancestor to dummy tuple (-1,-1,-1)
        for (int i=0;i<NumRow;++i) // add all point in subnet thay connect moved CI
        {
            for (int j=0;j<NumCol;++j)
            {
                for (int k=0;k<NumLyr;++k)
                {
                    if (GGridList[i][j][k].Covered == 1)
                    {
                        GGridList[i][j][k].Color = "G";
                        queue1.push_back(make_tuple(i+1,j+1,k+1));
                        GGridList[i][j][k].Step = 1;
                    }
                }
            }
        }
        int step = 1;
        while(!found) // while not found
        {
            ++step;
            for (auto& t : queue1) // Draw outer part
            {
                row = get<0>(t);
                col = get<1>(t);
                lyr = get<2>(t);
                if (LList_idx[lyr]->getDir() == 'H')
                {
                    if (col>1 && GGridList[row-1][col-2][lyr-1].getSupply()>1 && GGridList[row-1][col-2][lyr-1].Color == "W") // left GGrid
                    {
                        GGridList[row-1][col-2][lyr-1].Color = "G";
                        GGridList[row-1][col-2][lyr-1].Step = step;
                        queue2.push_back(make_tuple(row,col-1,lyr));
                        GGridList[row-1][col-2][lyr-1].Ancestor = t;
                    }
                    if (col<NumCol && GGridList[row-1][col][lyr-1].getSupply()>1 && GGridList[row-1][col][lyr-1].Color == "W") // right GGrid
                    {
                        GGridList[row-1][col][lyr-1].Color = "G";
                        GGridList[row-1][col][lyr-1].Step = step;
                        queue2.push_back(make_tuple(row,col+1,lyr));
                        GGridList[row-1][col][lyr-1].Ancestor = t;
                    }
                }
                else if (LList_idx[lyr]->getDir() == 'V')
                {
                    if (row>1 && GGridList[row-2][col-1][lyr-1].getSupply()>1 && GGridList[row-2][col-1][lyr-1].Color == "W") // front GGrid
                    {
                        GGridList[row-2][col-1][lyr-1].Color = "G";
                        GGridList[row-2][col-1][lyr-1].Step = step;
                        queue2.push_back(make_tuple(row-1,col,lyr));
                        GGridList[row-2][col-1][lyr-1].Ancestor = t;
                    }
                    if (row<NumRow && GGridList[row][col-1][lyr-1].getSupply()>1 && GGridList[row][col-1][lyr-1].Color == "W") // back GGrid
                    {
                        GGridList[row][col-1][lyr-1].Color = "G";
                        GGridList[row][col-1][lyr-1].Step = step;
                        queue2.push_back(make_tuple(row+1,col,lyr));
                        GGridList[row][col-1][lyr-1].Ancestor = t;
                    }
                }
                else
                {
                    cout << "wierd" << endl;
                }
                if (lyr>1 && lyr-1 >= minlyr && GGridList[row-1][col-1][lyr-2].getSupply()>1 && GGridList[row-1][col-1][lyr-2].Color == "W") // lower GGrid
                {
                    GGridList[row-1][col-1][lyr-2].Color = "G";
                    GGridList[row-1][col-1][lyr-2].Step = step;
                    queue2.push_back(make_tuple(row,col,lyr-1));
                    if (LList_idx[lyr]->getPF()>LList_idx[lyr-1]->getPF() || GGridList[row-1][col-1][lyr-2].Ancestor==make_tuple(-1,-1,-1))
                    {
                        GGridList[row-1][col-1][lyr-2].Ancestor = t;
                    }
                }
                if (lyr<NumLyr && GGridList[row-1][col-1][lyr].getSupply()>1 && GGridList[row-1][col-1][lyr].Color == "W") // upper GGrid
                {
                    GGridList[row-1][col-1][lyr].Color = "G";
                    GGridList[row-1][col-1][lyr].Step = step;
                    queue2.push_back(make_tuple(row,col,lyr+1));
                    if (LList_idx[lyr]->getPF()>LList_idx[lyr+1]->getPF() || GGridList[row-1][col-1][lyr-2].Ancestor==make_tuple(-1,-1,-1))
                    {
                        GGridList[row-1][col-1][lyr].Ancestor = t;
                    }
                }
            }
            for (auto& t : queue2) // check if other subnet is touched
            {
                row = get<0>(t);
                col = get<1>(t);
                lyr = get<2>(t);
                if (GGridList[row-1][col-1][lyr-1].Covered > 1) // if touched, throw it in queue3
                {
                    found = true;
                    queue3.push_back(t);
                }
            }
            if (found) // connect subnet
            {
                for (auto& t : queue3)
                {
                    queue4.clear();
                    row = get<0>(t);
                    col = get<1>(t);
                    lyr = get<2>(t);
                    int now_cover = GGridList[row-1][col-1][lyr-1].Covered;
                    if (now_cover == 1) // different point of the same subnet had been touched
                    {
                        continue;
                    }
                    for (int i=0;i<NumRow;++i) // set linked subnet's covered to 1
                    {
                        for (int j=0;j<NumCol;++j)
                        {
                            for (int k=0;k<NumLyr;++k)
                            {
                                if (GGridList[i][j][k].Covered == now_cover)
                                {
                                    GGridList[i][j][k].Covered = 1;
                                }
                            }
                        }
                    }
                    int temp_step = GGridList[row-1][col-1][lyr-1].Step;
                    tuple<int,int,int> tt = t;
                    while(temp_step >= 1)
                    {
                        queue4.push_back(tt);
                        row = get<0>(tt);
                        col = get<1>(tt);
                        lyr = get<2>(tt);
                        tt = (GGridList[row-1][col-1][lyr-1].Ancestor);
                        --temp_step;
                    }
                    if (queue4.size()>2)
                    {
                        for (int g=1;g<queue4.size()-1;++g)
                        {
                            int r1 = get<0>(queue4[g]) - get<0>(queue4[g-1]);
                            int c1 = get<1>(queue4[g]) - get<1>(queue4[g-1]);
                            int l1 = get<2>(queue4[g]) - get<2>(queue4[g-1]);
                            int r2 = get<0>(queue4[g+1]) - get<0>(queue4[g]);
                            int c2 = get<1>(queue4[g+1]) - get<1>(queue4[g]);
                            int l2 = get<2>(queue4[g+1]) - get<2>(queue4[g]);
                            if ((r1==r2 && r1!=0) || (c1==c2 && c1!=0) || (l1==l2 && l1!=0))
                            {
                                queue4.erase(queue4.begin()+g);
                            }
                        }
                        for (int g=0;g<queue4.size()-1;++g)
                        {
                            int row2 = get<0>(queue4[g]);
                            int col2 = get<1>(queue4[g]);
                            int lyr2 = get<2>(queue4[g]);
                            int row1 = get<0>(queue4[g+1]);
                            int col1 = get<1>(queue4[g+1]);
                            int lyr1 = get<2>(queue4[g+1]);
                            outputRoute.push_back(new Route(row1,col1,lyr1,row2,col2,lyr2,netname));
                        }
                    }
                    else if (queue4.size()==2)
                    {
                        int row2 = get<0>(queue4[0]);
                        int col2 = get<1>(queue4[0]);
                        int lyr2 = get<2>(queue4[0]);
                        int row1 = get<0>(queue4[1]);
                        int col1 = get<1>(queue4[1]);
                        int lyr1 = get<2>(queue4[1]);
                        outputRoute.push_back(new Route(row1,col1,lyr1,row2,col2,lyr2,netname));
                    }
                    else
                    {
                        cout << "wierd" << endl;
                    }
                }
            }
            else // update queue
            {
                queue1 = queue2;
                queue2.clear();
                if (!queue3.empty())
                {
                    cout << "wierd" << endl;
                    queue3.clear();
                }
            }
        }
        --count;
    }
    return outputRoute;
}

double Design::routing(string& CI, tuple<int, int> new_loc)
{
    cout << "Start routing..." << endl;
    // Relocate CI
    CIList[CI].Relocate(new_loc);
    // get subnets & link grid
    map<string,vector<Route*>> subnet;
    clearGGridCovered();
    clearGGridstep();
    double cost_org = 0;
    double cost_new = 0;
    int rr = get<0>(CIList[CI].getLocation());
    int cc = get<1>(CIList[CI].getLocation());
    int ll;
    // for every net
    for (auto& n : CIList[CI].getADJNets())
    {
        int subnetCount = NList[n].getCIs().size();
        int countidx = 1;
        string pname;
        clearGGridCovered();
        clearGGridstep();
        vector<Route*> RList = NList[n].getRList();
        // set visited for every pin and route
        for (auto& c : NList[n].getCIs())
        {
            for (auto& p : CIList[c].getPList())
            {
                if (p->getNetname() == n)
                { 
                    int row = get<0>(CIList[c].getLocation());
                    int col = get<1>(CIList[c].getLocation());
                    int lyr = p->getLayer().getIdx();
                    if (GGridList[row-1][col-1][lyr-1].Covered==0){
                        ++countidx;
                        setCICovered(RList,row,col,lyr,countidx);
                    }
                }
            }
        }
        for (auto& p : CIList[CI].getPList())
        {
            if (p->getNetname() == n)
            { 
                pname = p->getName();
                ll = p->getLayer().getIdx();
                if (GGridList[rr-1][cc-1][ll-1].Covered==0){
                    setCICovered(RList,rr,cc,ll,1);
                }
            }
        }
        // check covered result
        // cout << n << endl;
        // showCovered();
        // maze route
        vector<Route*> candidate = mst(countidx, rr, cc, ll, NList[n].getMinLyr(), NList[n].getName());
        double cost1 = calculate(NList[n].getR(), NList[n].getWeight());
        cost_org += cost1;
        double cost2 = calculate(candidate, NList[n].getWeight());
        cost_new += cost2;
        cout << "Original cost: " << cost1 << ", New cost: " << cost2 << "." << endl;
        // link
        if (cost1 > cost2)
        {
            NList[n].addtoRList(candidate);
            for (auto& r : candidate)
            {
                addRoute(r->RowS,r->RowE,r->ColS,r->ColE,r->LyrS,r->LyrE,n);
            }
        }
        else
        {
            NList[n].addtoRList(NList[n].getR());
            for (auto& r : NList[n].getR())
            {
                addRoute(r->RowS,r->RowE,r->ColS,r->ColE,r->LyrS,r->LyrE,n);
            }
        }
        NList[n].connect(CIList[CI],pname);
    }
    cout << "Routing finished. The benefit is " << (cost_new-cost_org) << "." << endl;
    return (cost_new - cost_org);
}

double Design::calculate(vector<Route*> R, double weight)
{
    // r : route list
    // weight : the weight of the net with this route list
    clearGGridCovered();
    for (auto& r : R)
    {
        setGGridCovered(r,1);
    }
    int num;
    vector<int> numlist;
    for (int k=0;k<NumLyr;++k)
    {
        num = 0;
        for(int i=0;i<NumRow;++i)
        {
            for (int j=0;j<NumCol;++j)
            {
                num += GGridList[i][j][k].Covered;
            }
        }
        numlist.push_back(num);
    }
    double cost = 0;
    for (int i = 0;i<NumLyr;++i)
    {
        cost += (numlist[i] * LList_idx[i+1]->getPF());
    }
    return cost * weight;
}