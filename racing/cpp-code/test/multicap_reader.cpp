#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "utilities.h"
#include "Starter.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include "statutils.h"
#include "horse.h"
#include <stdlib.h>
#include <stdio.h>
using namespace std;
const char* pp_dir = "/home/john/development/alogatas/horse-racing/Documents/PastPerformances/BrisFiles/2011";

void process_pp4(const std::string& filename)
{
    if(filename[0] == '.') {
        return;
    }

    int max = 0;
    ifstream myfile(filename.c_str());
    std::vector<horse*> horses;
    {
        while(myfile.good()) {
            horse* h = horse::get();
            h->populate(myfile);

            if(h->length() <= 0) {
                horse::put_back(h);
                continue;
            }

            horses.push_back(h);
            const int l = h->length();

            if(l > max) {
                max = l;
            }
        }

        myfile.close();
    }

    for(register int i = 0; i < horses.size(); ++i) {
        horse::put_back(horses[i]);
    }
}

int main()
{
    cout << timestamp() << endl;
    string dir = string(pp_dir);
    vector<string> files = vector<string>();
    getdir(dir, files);
    int count = 0;

    for(unsigned int i = 0; i < files.size(); i++) {
        if(files[i][0] == '.') {
            continue;
        }

        process_pp4(string(pp_dir) + string("/") + files[i]);
        ++count;

        if(count % 100 == 0) {
            std::cout <<    "count : " << count << std::endl;
        }
    }

    std::cout << "count : " << count << std::endl;
    std::cout << "number of files : " << files.size() << std::endl;
    cout << timestamp() << endl;
    return 0;
}
