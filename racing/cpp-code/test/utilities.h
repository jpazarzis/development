// utilities.h
// Author: delta
#ifndef UTILITIES_INCLUDED
#define UTILITIES_INCLUDED
#include <vector>
#include <string>

int getdir(const std::string& dir, std::vector<std::string> &files);

void parse_pp(char* psz, int* index);

std::string timestamp();

#endif // UTILITIES_INCLUDED

