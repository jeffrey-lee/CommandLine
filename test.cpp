#include "CommandLine.h"
#include <iostream>

using namespace std;

int main(){

    CommandLine cmd_line;
    while(1){
        std::string out_put = cmd_line.ReadCommand();
        cout <<  "read_command : " << out_put << endl;
    }

    return 0;
}
