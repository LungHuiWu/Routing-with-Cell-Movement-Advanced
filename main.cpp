#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "Design.h"
//hi
using namespace std;

int main(int argc, char** argv)
{
    // TODO
    // Parse the command and generate a design
    // argv[1]: input file name, argv[2]: output file name
    if (argc != 3)
    {
        cout << "Usage: ./cell_move_router <input.txt> <output.txt>";
    }
    else
    {
        Design D = Design();
        D.readFile(argv[1]);
        int cnt = 0; int r_num = 0;
        map<string, tuple<int,int>> MovedCI;
        while(cnt<D.getMax()){   
            string s = D.select();
            cout << "The selected cell is " << s << "." << endl;
            vector<tuple<int, int>> c = D.placement(s);
            for (int i = 0; i<2; i++)
            {
                if (i==0)
                {
                    cout << "The first new place of " << s << " is (" << get<0>(c[i]) << "," << get<1>(c[i]) << ")." << endl;
                }
                else if (i==1)
                {
                    cout << "The second new place of " << s << " is (" << get<0>(c[i]) << "," << get<1>(c[i]) << ")." << endl;
                }   
            }
            if(D.routing(s,c[0],1) <= 0) // routing output is the benefit
            {
                MovedCI[s] = c[0];
                cout << "Routing successful at the first location!" << endl;
            }
            else {
                if (D.routing(s,c[1],2) < 0) // So far haven't get here, but delroute is needed if in here again
                {
                    MovedCI[s] = c[1];
                    cout << "Routing successful at the second location!" << endl;
                }
                else
                {
                    cout << "New location sucks QAQ" << endl;
                }
            }
            ++cnt;
        }  
        fstream f;
        f.open(argv[2], ios::out);
        if (!f)
        {
            cerr << "fuck you" << endl;
        }
        else
        {
            f << "NumMovedCellInst " << MovedCI.size() << endl;
            for (auto& c : MovedCI)
            {
                f << "CellInst " << c.first << " " << get<0>(c.second) << " " << get<1>(c.second) << endl;
            }
            for (auto& n : D.getNList())
            {
                r_num += n.second.getRList().size();
            }
            f << "NumRoutes " << r_num << endl;
            for (auto& n : D.getNList())
            {
                for (int rrr=0;rrr < n.second.getRList().size();++rrr)
                {
                    f << n.second.getRList()[rrr]->RowS << " " << n.second.getRList()[rrr]->ColS << " " << n.second.getRList()[rrr]->LyrS << " " << n.second.getRList()[rrr]->RowE << " " << n.second.getRList()[rrr]->ColE << " " << n.second.getRList()[rrr]->LyrE << " " << n.first << endl;
                }
            }
        }
    }
    return 1;
}