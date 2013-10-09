#include "CloneableDouble.h"
#include "std_include.h"

using namespace std;

#define MIN_ACCEPTABLE_CHROMOSOME_LENGTH 8

#define CHROMOSOME_LENGTH sizeof(ULONG) * 8

CloneableDouble::CloneableDouble(double min, double max, int decimals):
    _min(min), 
    _max(max), 
    _decimals(decimals), 
    _max_length(long_to_bits((_max - _min )* pow(10, _decimals)).length())
{
    assert(_min >= 0);
    assert(_min < max);
    assert(_decimals >= 0);
    assert(_max_length >= MIN_ACCEPTABLE_CHROMOSOME_LENGTH);

    double d_value = (_max - _min) * ( (double)rand() / (double)RAND_MAX );
    _long_value= d_value * pow(10, _decimals);
}

int CloneableDouble::max_length() const
{
    return _max_length;
}

double CloneableDouble::get_value() const
{
    return (_long_value*1.0) / pow(10, _decimals) + _min;
}

std::string CloneableDouble::to_binary_string() const
{
    std::string bits = long_to_bits(_long_value);

    while(bits.length() < _max_length)
    {
        bits = "0" + bits;
    }
    return bits;
}

void CloneableDouble::read_from_double(double v)
{
    assert(v>0);
    _long_value= (v - _min) * pow(10, _decimals);
}

std::string CloneableDouble::read_from_binary(const std::string& bin_string)
{
    assert(bin_string.length() >=max_length());
    _long_value = bits_to_long(bin_string.substr(0,max_length()));
    return bin_string.substr(max_length());
}

CloneableDouble::~CloneableDouble()
{
}

CloneableDouble::CloneableDouble(const CloneableDouble& other):
     _min(other._min),
     _max(other._max),
     _decimals(other._decimals),
     _max_length(other._max_length),
     _long_value(other._long_value)
{
}

std::string CloneableDouble::to_string() const 
{
    char buffer[1024];
    sprintf(buffer, "CloneableDouble: %10.6f max. length %d" , get_value(), _max_length);
    return buffer;
}

std::string long_to_bits(ULONG l)
{
    char bin[CHROMOSOME_LENGTH];
    int starting_at = -1;
    register int p = 0;
    for(register int i = CHROMOSOME_LENGTH-1; i >= 0; --i)
    {
        bin[p] = '0';
        const ULONG current_power = pow(2,i);
        if( (current_power & l) == current_power)
        {
            bin[p] = '1';
            if(starting_at < 0)
            {
                starting_at = p;
            }
        }
        p++;
    }

    if(starting_at < 0)
    {
        starting_at = CHROMOSOME_LENGTH-1;
    }

    return &bin[starting_at];
}

ULONG bits_to_long(const std::string& bits)
{
    const int length = bits.length();
    assert(length<=CHROMOSOME_LENGTH);
    ULONG value = 0;
    register int power = length-1;
    for(register int i = 0; i <length; ++i)
    {
        if(bits[i] == '1')
        {
            value += pow(2,power); // optimize this
        }
        --power;
    }
    return value;
}


void test_bits_to_long()
{
    assert(bits_to_long("0") == 0);
    assert(bits_to_long("1") == 1);
    assert(bits_to_long("10") == 2);
    assert(bits_to_long("11") == 3);
    assert(bits_to_long("100") == 4);
    assert(bits_to_long("101") == 5);
    assert(bits_to_long("110") == 6);
    assert(bits_to_long("111") == 7);
    assert(bits_to_long("1000") == 8);
    assert(bits_to_long("1001") == 9);
    assert(bits_to_long("1010") == 10);
    assert(bits_to_long("10101011") == 171);
    assert(bits_to_long("11100011100000111101011") == 7455211);
}

void test_long_to_bits()
{
    assert(long_to_bits(0).compare("0") == 0);
    assert(long_to_bits(1).compare("1") == 0);
    assert(long_to_bits(2).compare("10") == 0);
    assert(long_to_bits(3).compare("11") == 0);
    assert(long_to_bits(4).compare("100") == 0);
    assert(long_to_bits(5).compare("101") == 0);
    assert(long_to_bits(6).compare("110") == 0);
    assert(long_to_bits(7).compare("111") == 0);
    assert(long_to_bits(8).compare("1000") == 0);
    assert(long_to_bits(9).compare("1001") == 0);
    assert(long_to_bits(10).compare("1010") == 0);
    assert(long_to_bits(171).compare("10101011") == 0);
    assert(long_to_bits(7455211).compare("11100011100000111101011") == 0);     
}
