#include "utilities.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

std::string timestamp()
{
    time_t rawtime;
    time(&rawtime);
    return ctime(&rawtime);
}

int getdir(const std::string& dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }

    closedir(dp);
    return 0;
}

void parse_pp(char* psz, int* index)
{
    const int length = strlen(psz);

    if(0 == psz ||  length == 0) {
        return;
    }

    bool inside_str = false;
    int field_index = 0, field_begining = 0;

    for(register int i = 0; i < length; ++i) {
        if(psz[i] == '"') {
            inside_str = !inside_str;
        }

        if(false == inside_str && psz[i] == ',') {
            psz[i] = '\0';
            index[field_index] = field_begining;
            field_begining = i + 1;
            ++field_index;
        }
    }

    index[field_index] = field_begining;
}
