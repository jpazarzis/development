// test_construction_boost_date.cpp
// tests the creation of a boost date from the custom generator
//
#include "toolkit.h"
#include <iostream>
#include <assert.h>
using namespace std;

int main(){

    auto d = make_date("20121211");
    assert(d.year() == 2012 && d.month() == 12 && d.day() == 11);

    d = make_date("20130101");
    assert(d.year() == 2013 && d.month() == 1 && d.day() == 1);

    d = make_date("06/08/2011");
    assert(d.year() == 2011 && d.month() == 6 && d.day() == 8);

    d = make_date("junk");
    assert(d.is_not_a_date());

    d = make_date("2013k0101");
    assert(d.is_not_a_date());

    d = make_date("");
    assert(d.is_not_a_date());

    d = make_date("99999999");
    assert(d.is_not_a_date());
}
