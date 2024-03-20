#include <iostream>
#include "stropt.h"
#include "cstring"
using namespace std;

void deletelchr(char *str)
{
    int s = strlen(str);
    cout << s << endl;
}


int main() {
    string buffer="messi~!~10~!~true~!~a30~!~68.5~!~2100000~!~Barc,elona";
    ccmdstr cmdstr(buffer,"~!~");
    cout << cmdstr;
    return 0;
}
