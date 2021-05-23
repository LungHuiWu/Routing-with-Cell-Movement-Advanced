#include <string>
#include "Pin.h"
#include "Blkg.h"

using namespace std;

class MCell
{
private:
    string MCName;
    int NumPin;
    int NumBlkg;
    Pin* pin1; // linked list
    Blkg* blkg1; // linked list
    
public:
    MCell();
    MCell(string, int, int);
    ~MCell();
};

