#include "Identifiable.h"
long Identifiable::_seed = 10000;
Identifiable::Identifiable():
    _id(++_seed)
{
}

Identifiable::~Identifiable()
{
}
long Identifiable::get_id() const
{
    return _id;
}
