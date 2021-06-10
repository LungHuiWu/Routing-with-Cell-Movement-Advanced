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
        cout << "Route added to Net " << n << "." << endl;
    }
}

string Design::select()
{   
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
            maxWeight = Weight;
            CI = c.second.getCIName();
            //cout <<"current CI is "<<CI<<" with weight "<<Weight<<"."<<endl;
            }
    }
    mCIList.erase(CI);
    ADJCIs = CIList[CI].getADJCIs(NList);
    adjNets = CIList[CI].getADJNets();
    //delete RList in CI
    for(int i = 0; i<CIList[CI].getPList().size(); i++)
    {
        string net = CIList[CI].getPList()[i]->getNetname();
        //CIList[CI].getPList()[i]->Disconnect();
        NList[net].Disconnect(CIList[CI],CIList[CI].getPList()[i]->getName());
        NList[net].delRoute(CIList[CI]); 
    }
    return CI;
}

vector<tuple<int,int>> Design::placement(string& CI)
{   
    //case1 : CI doesn't have votage area, do BFS
    if(CIList[CI].hasVtgArea() == false)
    {
        int numinp = 0;
        vector<tuple<int,int>> p;
        //vector<vector<tuple<int,int>>> subnet;
        //cout<<subnet.size()<<endl;
        int nsize=adjNets.size();
        vector<vector<tuple<int,int>>> subnet(nsize);
        vector<vector<tuple<int,int>>> visited(nsize);
        for(int N=0; N<adjNets.size(); N++)
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
        while(numinp < 2)
        {
            for(int N=0; N<subnet.size(); N++)
            {
                int l=0;
                int loop=subnet[N].size();
                while(l<loop)
                {
                    int x=get<0>(subnet[N][l]);
                    int y=get<1>(subnet[N][l]);
                    if(x<RowMax)
                    {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x+1, y))==subnet[N].end())
                        {
                            if(find(visited[N].begin(), visited[N].end(), make_tuple(x+1, y))==visited[N].end()) 
                            {
                                subnet[N].push_back(make_tuple(x+1, y));
                                visited[N].push_back(make_tuple(x+1, y));
                            }
                        }
                    }
                    if(x>RowMin)
                    {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x-1, y))==subnet[N].end())
                        {
                            if(find(visited[N].begin(), visited[N].end(), make_tuple(x-1, y))==visited[N].end()) 
                            {
                                    subnet[N].push_back(make_tuple(x-1, y));
                                visited[N].push_back(make_tuple(x-1, y));
                            }
                        }
                    }
                    if(y<ColMax)
                    {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x, y+1))==subnet[N].end())
                        {
                            if(find(visited[N].begin(), visited[N].end(), make_tuple(x, y+1))==visited[N].end()) 
                            {
                                subnet[N].push_back(make_tuple(x, y+1));
                                visited[N].push_back(make_tuple(x, y+1));
                            }
                        }
                    }
                    if(y>ColMin)
                        {
                        if(find(subnet[N].begin(), subnet[N].end(), make_tuple(x, y-1))==subnet[N].end())
                        {
                            if(find(visited[N].begin(), visited[N].end(), make_tuple(x, y-1))==visited[N].end()) 
                            {
                                subnet[N].push_back(make_tuple(x, y-1));
                                visited[N].push_back(make_tuple(x, y-1));
                            }
                        }
                    }
                    visited[N].push_back(make_tuple(x, y));
                    l++;
                }
                subnet[N].erase(subnet[N].begin(), subnet[N].begin()+loop);
            }
            for(int i=0; i<visited[0].size(); i++)
            {
                if(numinp==2)
                {
                    break;
                }
                int overlap=1;
                for(int j=1; j<visited.size(); j++)
                {
                    if(find(visited[j].begin(), visited[j].end(), visited[0][i])==visited[j].end())
                    {
                        overlap=0;
                    }
                }
                if(overlap==1 && find(p.begin(), p.end(), visited[0][i])==p.end())
                {
                    if(true)//supply>demand
                    {
                        p.push_back(visited[0][i]);
                        numinp += 1;
                    }
                }
            }
        }
        cout<<CI<<" doesn't has votage area."<<endl;
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

void Design::routing(string& newCI, tuple<int, int> new_loc, vector<string>& net)
{
    for(size_t i=0; i<net.size();++i)
    {
        vector<string> allCI = NList[net[i]].getCIs();
    }
    return;
}

double Design::calculate(vector<Route*>& r, double weight)
{
    // r : route list
    // weight : the weight of the net with this route list
    int X = NumRow;
    int Y = NumCol;
    int Z = NumLyr+1;

    bool ***arr = new bool**[X];
    vector<int> countEachLyr(Z,0);
    for(int i =0; i<X; i++){
        arr[i] = new bool*[Y];
        for(int j =0; j<Y; j++){
            arr[i][j] = new bool[Z];
            for(int k = 0; k<Z;k++){
                arr[i][j][k] = false;
            }
        }
    }
    for(size_t i=0;i<r.size();++i)
    {
        int r1 = min(r[i]->RowS, r[i]->RowE);
        int r2 = max(r[i]->RowS, r[i]->RowE);
        int c1 = min(r[i]->ColS, r[i]->ColE);
        int c2 = max(r[i]->ColS, r[i]->ColE);
        int l1 = min(r[i]->LyrS, r[i]->LyrS);
        int l2 = max(r[i]->LyrS, r[i]->LyrS);
        if(r1 != r2)
        {
            for (int i=r1;i<r2;++i)
            {
                if(!arr[i][c1][l1])
                {
                    arr[i][c1][l1] = true;
                    countEachLyr[l1] += 1;
                }
            }
        }
        else if(c1 != c2)
        {
<<<<<<< HEAD
            for (int i=c1;i<c2;++i)
            {
                if(!arr[r1][i][l1])
                {
                    arr[r1][i][l1] = true;
                    countEachLyr[l1] += 1;
                }
            }
        }
        else if(l1 != l2)
=======
            mindis = dis;
            p[0] = Vtgarea[i];
        }
        else if(dis>=mindis && dis<=secmindis)
>>>>>>> e271ea4cbe59c9cb85c2825563bfccd0a06864af
        {
            for (int i=l1;i<l2;++i)
            {
                if(!arr[r1][c1][i])
                {
                    arr[r1][c1][i] = true;
                    countEachLyr[i] += 1;
                }
            }
        }
        else cout << "Wrong Route!!" << endl;
    }
<<<<<<< HEAD
=======
    return p;
}

void Design::routing(string& newCI, tuple<int, int> new_loc, vector<string>& net)
{
    for(size_t i=0; i<net.size();++i)
    {
        vector<string> allCI = NList[net[i]].getCIs();
    }
    return;
}

double Design::calculate(vector<Route*>& r, double weight)
{
    // r : route list
    // weight : the weight of the net with this route list
    int X = NumRow;
    int Y = NumCol;
    int Z = NumLyr+1;

    bool ***arr = new bool**[X];
    vector<int> countEachLyr(Z,0);
    for(int i =0; i<X; i++){
        arr[i] = new bool*[Y];
        for(int j =0; j<Y; j++){
            arr[i][j] = new bool[Z];
            for(int k = 0; k<Z;k++){
                arr[i][j][k] = false;
            }
        }
    }
    for(size_t i=0;i<r.size();++i)
    {
        int r1 = min(r[i]->RowS, r[i]->RowE);
        int r2 = max(r[i]->RowS, r[i]->RowE);
        int c1 = min(r[i]->ColS, r[i]->ColE);
        int c2 = max(r[i]->ColS, r[i]->ColE);
        int l1 = min(r[i]->LyrS, r[i]->LyrS);
        int l2 = max(r[i]->LyrS, r[i]->LyrS);
        if(r1 != r2)
        {
            for (int i=r1;i<r2;++i)
            {
                if(!arr[i][c1][l1])
                {
                    arr[i][c1][l1] = true;
                    countEachLyr[l1] += 1;
                }
            }
        }
        else if(c1 != c2)
        {
            for (int i=c1;i<c2;++i)
            {
                if(!arr[r1][i][l1])
                {
                    arr[r1][i][l1] = true;
                    countEachLyr[l1] += 1;
                }
            }
        }
        else if(l1 != l2)
        {
            for (int i=l1;i<l2;++i)
            {
                if(!arr[r1][c1][i])
                {
                    arr[r1][c1][i] = true;
                    countEachLyr[i] += 1;
                }
            }
        }
        else cout << "Wrong Route!!" << endl;
    }
>>>>>>> e271ea4cbe59c9cb85c2825563bfccd0a06864af
    double cost = 0;
    for(auto& l : LList)
    {
        int lnum = l.second->getIdx();
        cost += countEachLyr[lnum] * l.second->getPF();
    }
    return cost * weight;
}