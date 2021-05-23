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
    vector<Pin> Plist; // linked list
    vector<Blkg> Blist; // linked list
    
public:
    MCell();
    MCell(string, int, int);
    ~MCell();
    string getName();
    void addPin(string, Layer);
    void addBlkg(string, Layer, int);
    vector<Pin> getPList();
    vector<Blkg> getBList();
};

