#ifndef MCELL_H
#define MCELL_H
#include <string>
#include <vector>
#include "Pin.h"
#include "Blkg.h"

using namespace std;

class MCell
{
private:
    string MCName;
    int NumPin;
    int NumBlkg;
    vector<Pin*> PList;
    vector<Blkg*> BList;
    
public:
    MCell();
    MCell(string, int, int);
    ~MCell();
    string getName();
    void addPin(string, Layer);
    void addBlkg(string, Layer, double);
    vector<Pin*> getPList();
    vector<Blkg*> getBList();
    MCell& operator = (const MCell&);
};

#endif // MCELL_H