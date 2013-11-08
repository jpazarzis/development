// statutils.h
#ifndef STATUTILS_INCLUDED
#define STATUTILS_INCLUDED
#include <vector>
template<class T> T avg(const std::vector<T>& v)
{
    T sum = 0;
    const size_t l = v.length();

    for(register size_t i = 0; i < l; sum += v[i++]);

    return l > 0 ? sum / l : 0;
}
#endif // STATUTILS_INCLUDED
